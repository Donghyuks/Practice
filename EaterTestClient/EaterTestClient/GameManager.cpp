
#include "GameManager.h"
#include "DHKeyIO.h"
#include "DHTimer.h"
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "WorldData_generated.h"
#include "PortIPDefine.h"
#include "math.h"
#include "PhysX_API.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance)
{
	Prev_Time = std::chrono::system_clock::now();
	Current_Time = std::chrono::system_clock::now();
	m_KeyIO = new DHKeyIO();
	m_RTT = (double)1.f / (double)SERVER_FREQUENCY;

	// 게임의 전체적인 프레임
	m_Timer = new DHTimer();
	m_Timer->SetFrame(5000);

	// Key Input 타이머
	m_KeyIO_Timer = new DHTimer();
	m_KeyIO_Timer->SetFrame(60);

	_Send_Packet = new C2S_Packet();

	/// Win32 관련
	// 윈도 클래스
	LPCWSTR WinName = L"Eater Client Demo";
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = GameManager::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = WinName;

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 윈도 생성
	g_hwnd = CreateWindowW(WinName, WinName, WS_OVERLAPPEDWINDOW,
		0, 0, WORLD_SCREEN_X, WORLD_SCREEN_Y, nullptr, nullptr, hInstance, nullptr);

	if (!g_hwnd) return S_FALSE;

	scanf_s("%d", &m_Key);


	// PhysX
//#ifdef _DEBUG
//	PhysX_Initialize(4, nullptr, false);
//#else
//	PhysX_Initialize(4, nullptr, false);
//#endif

	if (m_Key == 0)
	{
		//PhysX_Initialize(4, nullptr, true);
		PhysX_Initialize(4, nullptr, false);
	}
	else
	{
		//PhysX_Initialize(4, nullptr, true);
		PhysX_Initialize(4, nullptr, false);
	}

	// 플레이어 데이터.
	m_Dest_Player = new Player();
	m_Dest_Player->m_PhysX = new PhysData();
	m_Dest_Player->m_PhysX->SetLockAxis_Position(false, true, false);
	m_Dest_Player->m_PhysX->SetLockAxis_Rotation(true, true, true);


	// 서버 연결..
	m_Network = new DHNetWorkAPI();
	m_Network->Initialize(DHNetWork_Name::DHNet);
	while (!m_Network->Connect(GAME_SERVER_PORT_1, LOCAL_CONNECT_IP)) {};

	// 그래픽스 엔진 초기화
	MYD2D->Initialize(g_hwnd);

	// 생성된 윈도를 화면에 표시
	ShowWindow(g_hwnd, SW_SHOWNORMAL);
	UpdateWindow(g_hwnd);

	MYD2D->SetGridMod(true, 100.0f, 100.0f, D2D1::ColorF(0.5f, 0.5f, 0.5f));

	// 로딩완료
	_Send_Packet->Packet_Type = C2S_LOADING_COMPLETE_REQ;
	_Send_Packet->Packet_Size = sizeof(m_Key);
	memcpy_s(_Send_Packet->Packet_Buffer, sizeof(m_Key), &m_Key, sizeof(m_Key));
	m_Network->Send(_Send_Packet);

	return S_OK;
}

void GameManager::Loop()
{
	while (true)
	{
		m_Timer->Update();
		m_KeyIO_Timer->Update();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			DispatchMessage(&msg);
		}
		else
		{
			if (m_KeyIO_Timer->Ready_Frame())
			{
				KeyIO_Recording();
				PhysX_World_Update();
			}

			if (m_Timer->Ready_Frame())
			{
				m_Dtime = m_Timer->GetFrameTime();
				GameLoop();
			}
		}

		Sleep(0);
	}
}

void GameManager::GameLoop()
{
	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;
	m_Client_Move_Vector = { 0,0,0 };
	m_Passed_Client_Time += m_Dtime;

	/*
		기본적인 클라이언트 동작 순서.
			1.		서버로부터 상태를 업데이트 받고 체크한다.
			2-1.	Input을 받는다. (60FPS 으로 받음)
			2-2.	PhysX World를 업데이트 한다.
			3.		보간을 적용한다.
			4.		Render 한다.
	*/

	/// 1. 서버로부터 상태를 업데이트 받고 체크한다.
	if (m_Network->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

			if (m_Is_Start == false)
			{
				// 처음에 내가 몇번째 유저인지 알려준다.
				if (Recv_Packet->Packet_Type == S2C_LOADING_COMPLETE_RES)
				{
					// 데이터 캐스팅.
					unsigned short* Recv_Player_Index = (unsigned short*)Recv_Packet->Packet_Buffer;

					// 서버에서 알려준 자신의 플레이어 번호.
					m_Player_Number = *Recv_Player_Index;
				}

				// 게임 시작시 서버로 부터 받아오는 캐릭터나 속도 정보들.
				if (Recv_Packet->Packet_Type == S2C_START_GAME)
				{
					// 데이터 캐스팅.
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					const Eater::GameData::StartInfo* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::StartInfo>(Recv_Data_Ptr);

					auto _Players_Data = _Recv_Data->users();

					for (int i = 0; i < _Players_Data->size(); i++)
					{
						auto Player_Data = _Players_Data->Get(i);

						auto _Player_Index = Player_Data->playerindex();

						// 유저를 등록
						Player* _Player = new Player();
						_Player->m_PhysX = new PhysData();
						m_Player_List.insert({ _Player_Index , _Player });

						// 서버로 부터 받아온 캐릭터 종류와 속도.
						m_Player_List[_Player_Index]->m_Character = Player_Data->character_type();
						m_Player_List[_Player_Index]->m_Speed = Player_Data->world_speed();
						m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(Player_Data->world_position()->x(),
							Player_Data->world_position()->y(), Player_Data->world_position()->z());

						m_Player_List[_Player_Index]->m_Prev_Position = m_Player_List[_Player_Index]->m_Position;

						// 현재 유저가 누구인지 등록
						if (_Player_Index == m_Player_Number)
						{
							m_Current_Player = m_Player_List[m_Player_Number];
							m_Dest_Player->m_PhysX->SetWorldPosition(m_Current_Player->m_Position.x, m_Current_Player->m_Position.y, m_Current_Player->m_Position.z);
							PhysX_Create_Actor(m_Dest_Player->m_PhysX);
						}
						/// 임시로 상대유저는 동적인 메쉬로 등록해놓음
						else
						{
							m_Player_List[_Player_Index]->m_PhysX->SetLockAxis_Position(false, true, false);
							m_Player_List[_Player_Index]->m_PhysX->SetLockAxis_Rotation(true, true, true);
							m_Player_List[_Player_Index]->m_PhysX->SetWorldPosition(Player_Data->world_position()->x(),
								Player_Data->world_position()->y(), Player_Data->world_position()->z());
							PhysX_Create_Actor(m_Player_List[_Player_Index]->m_PhysX);
						}
					}

					m_Is_Start = true;
				}
			}
			else
			{
				// 서버에서 주기적으로 보내오는 World 상태.
				if (Recv_Packet->Packet_Type == S2C_WORLD_UPDATE)
				{
					Enemy_List.clear();
					Mana_List.clear();

					Current_Time = std::chrono::system_clock::now();

					Passed_Time = Current_Time - Prev_Time;

					Prev_Time = Current_Time;

					// 서버로부터 데이터를 받았으므로, 대기시간 초기화.
					m_Passed_Client_Time = 0.f;

					// 데이터 캐스팅.
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					const Eater::GameData::WorldData* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::WorldData>(Recv_Data_Ptr);

					// 적에 대한 정보
					auto _Enemy_Data = _Recv_Data->enemies();
					for (int i = 0; i < _Enemy_Data->size(); i++)
					{
						auto _Enemy = _Enemy_Data->Get(i);
						auto _Position = _Enemy->position();

						Enemy_List.push_back({ _Enemy->index(), DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z()) });
					}

					// 맵에 오브젝트 (마나석 정보)
					auto _Object_Data = _Recv_Data->objects();
					for (int i = 0; i < _Object_Data->size(); i++)
					{
						auto _Object = _Object_Data->Get(i);
						auto _Position = _Object->position();

						Mana_List.push_back({ _Object->index(), DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z()) });
					}
					
					// 유저정보에 대한 부분
					auto _Players_Data = _Recv_Data->users();

					for (int i = 0; i < _Players_Data->size(); i++)
					{
						auto Player_Data = _Players_Data->Get(i);

						// 유저 번호.
						auto _Player_Index = Player_Data->playerindex();
						// 서버에서 처리한 시퀀스 번호
						auto _Sequence_Num = Player_Data->sequence();
						// Animation
						auto _Animation = Player_Data->animation();
						// Position
						auto _Position = Player_Data->position();
						// Mov_Vec
						auto _Mov_Vec = Player_Data->mov_vector();

						// 현재 플레이중인 데이터
						if (_Player_Index == m_Player_Number)
						{
							while (!m_Client_Predict.empty())
							{
								// 큐에서 데이터를 순서대로 뺴옴
								FrameData* _FData = m_Client_Predict.front();
								m_Client_Predict.pop();

								// 만약 해당 시퀀스까지 처리가 끝난 데이터가 서버로부터 넘어오고, 해당데이터가 오차범위를 벗어난 경우 복구한다.
								if (_Sequence_Num == _FData->m_Sequence)
								{
									// 오차범위 검사
									if (abs(_Position->x() - _FData->m_Predict_Position.x) > m_Epsilon ||
										abs(_Position->z() - _FData->m_Predict_Position.z) > m_Epsilon)
									{
										// 서버로 부터 받은 데이터로 백업 후 상태를 재 시뮬레이션 한다.
										m_Dest_Player->m_PhysX->SetWorldPosition(_Position->x(), _Position->y(), _Position->z());
										m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;
										m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec->x(), _Mov_Vec->y(), _Mov_Vec->z());

										// 정의된 dtime 을 기준으로하여 물리 연산 업데이트.
										//PhysX_Update(PHYSX_UPDATE_TIME);
										//PhysX_Update_Actor(m_Dest_Player->m_PhysX);

										m_Client_BackUp = m_Client_Predict;

										while (!m_Client_BackUp.empty())
										{
											// 큐에서 데이터를 순서대로 뺴옴
											FrameData* _BackUp_FData = m_Client_BackUp.front();
											m_Client_BackUp.pop();

											// 백업 플레이어의 속도를 주고, 물리 시뮬레이션을 지정하여, 프레임의 데이터값을 수정한다.
											m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec->x() + _BackUp_FData->m_Mov_Vec3.x, 
												_BackUp_FData->m_Mov_Vec3.y + _Mov_Vec->y(),
												_BackUp_FData->m_Mov_Vec3.z + _Mov_Vec->z());
											
											// 정의된 dtime 을 기준으로하여 물리 연산 업데이트.
											PhysX_Update(PHYSX_UPDATE_TIME);
											PhysX_Update_Actor(m_Dest_Player->m_PhysX);

											m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;
											// 예측 위치 변경.
											_BackUp_FData->m_Predict_Position = m_Dest_Player->m_Position;
										}

										// 사용한 데이터 해제
										delete _FData;
										_FData = nullptr;
										break;
									}
								}

								// 사용한 데이터 해제
								delete _FData;
								_FData = nullptr;
							}

						}
						// 다른 클라이언트 데이터는 서버에서 받아온 데이터값을 쓴다.
						else
						{
							// 이전 포지션값 백업 (이전프레임)
							m_Player_List[_Player_Index]->m_Prev_Position = m_Player_List[_Player_Index]->m_Position;
							m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z());
							m_Player_List[_Player_Index]->m_Animation = _Animation;
							m_Player_List[m_Player_Number]->m_PhysX->SetWorldPosition(_Position->x(), _Position->y(), _Position->z());
							m_Player_List[m_Player_Number]->m_PhysX->SetVelocity(_Mov_Vec->x(), _Mov_Vec->y(), _Mov_Vec->z());
						}
					}
				}
			}

			// 사용한데이터 해제.
			delete S2C_Msg.Packet;
			S2C_Msg.Packet = nullptr;
		}
		Msg_Vec.clear();
	}

	// 게임이 시작되지 않았다면 아무것도 하지않음..
	if (m_Is_Start == false) return;


	/// 3,4 보간을 적용한 후 랜더링.
	MYD2D->StartRender();
	MYD2D->thDrawText(10, 10, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Current FPS : %.2f Fps", (double)1 / m_Dtime );
	MYD2D->thDrawText(10, 30, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Real RTT : %.2f s", Passed_Time.count());
	MYD2D->thDrawText(10, 50, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Setting RTT : %.2f s", m_RTT );
	MYD2D->thDrawText(10, 70, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Passed Client Time : %.2f s", m_Passed_Client_Time);

	float Scaleing = 10.f;

	// 마나스톤 그리기
	for (auto _Mana_Stone : Mana_List)
	{
		// 2D 상 쉽게 보이기위해 보정
		DirectX::SimpleMath::Vector3 _ADD_Pos(100, 0, 250);
		DirectX::SimpleMath::Vector3 _Draw_Pos = (_Mana_Stone.Position * Scaleing) + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Mana_Stone.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z , 1 * Scaleing, 1, D2D1::ColorF(0.0f, 0.0f, 1.0f));
	}

	// 적 그리기
	for (auto _Enemy : Enemy_List)
	{
		// 2D 상 쉽게 보이기위해 보정
		DirectX::SimpleMath::Vector3 _ADD_Pos(100, 0, 250);
		DirectX::SimpleMath::Vector3 _Draw_Pos = (_Enemy.Position * Scaleing) + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Enemy.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z, 1 * Scaleing, 1, D2D1::ColorF(1.0f, 0.0f, 1.0f));
	}

	// 보간을 적용할 포지션
	DirectX::SimpleMath::Vector3 _Interpolation_Pos;

	// 보간계수 (서버의 통신시간 (20프레임) 으로 현재 클라이언트가 네트워크로부터 데이터를 받은후 부터 지난 시간을 나눈다 )
	double _Interpolation = m_Passed_Client_Time / m_RTT;
	if (_Interpolation >= 1.0f) _Interpolation = 1.0f;

	/// 좌표상 보여주기 편하기위해서 좌표보정.
	DirectX::SimpleMath::Vector3 _Offset_Vec(100, 0, 250);

	/// 상대유저 (보간)

	for (auto _Other : m_Player_List)
	{
		// 만약 현재 유저면 건너뜀..
		if (_Other.second == m_Current_Player) continue;

		Player* _Other_Player = _Other.second;

		_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(_Other_Player->m_Prev_Position, _Other_Player->m_Position, _Interpolation);
		_Interpolation_Pos *= Scaleing;
		_Interpolation_Pos += _Offset_Vec;

		if (_Other_Player->m_Animation == ANIMATION_IDLE)
		{
			// 상대 유저는 가만히 있을때 빨간색 원 움직일때 빨간색 네모
			MYD2D->thDrawEllipse(_Interpolation_Pos.x, _Interpolation_Pos.z, 1 * Scaleing, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
		else if (_Other_Player->m_Animation == ANIMATION_RUN)
		{
			// 상대 유저는 가만히 있을때 빨간색 원 움직일때 빨간색 네모
			MYD2D->thDrawRectangle(_Interpolation_Pos.x - 4, _Interpolation_Pos.z - 4,
				_Interpolation_Pos.x + 4, _Interpolation_Pos.z + 4, 0.f, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
	}

	/// 현재유저 (예측)

	_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(m_Current_Player->m_Position, m_Dest_Player->m_Position, _Interpolation);
	_Interpolation_Pos *= Scaleing;
	_Interpolation_Pos += _Offset_Vec;

	if (m_Current_Player->m_Animation == ANIMATION_IDLE)
	{
		// 가만히 있는 경우 초록색 원
		MYD2D->thDrawEllipse(_Interpolation_Pos.x, _Interpolation_Pos.z, 1 * Scaleing, 2, D2D1::ColorF(0.0f, 1.0f, 0.0f));
	}
	else if (m_Current_Player->m_Animation == ANIMATION_RUN)
	{
		// 움직이는 경우 빨간색 네모
		MYD2D->thDrawRectangle(_Interpolation_Pos.x - 20, _Interpolation_Pos.z - 20,
			_Interpolation_Pos.x + 20, _Interpolation_Pos.z + 20, 0.f, 2, D2D1::ColorF(0.0f, 1.0f, 0.0f));
	}

	MYD2D->EndRender();
}

void GameManager::KeyIO_Recording()
{
	if (m_Is_Start == false)
	{
		return;
	}

	/// 2. Input을 받는다. ( 받은 Input을 네트워크에 메세지 전송 )
	// 서버에 Input값 전송을 위한 빌더
	flatbuffers::FlatBufferBuilder _Builder;

	// 이번 프레임에 대한 데이터 생성 후 큐잉.
	_FData = new FrameData();
	_FData->m_Sequence = m_Player_List[m_Player_Number]->m_Sequence;

	/// 키 입력을 기록해 둔다.
	DirectX::SimpleMath::Vector3 _Mov_Vec = DirectX::SimpleMath::Vector3(0,0,0);
	// 1p 조작
	if (m_Player_Number == EATER_PLAYER_01)
	{
		if (m_KeyIO->Key_Press(VK_RIGHT))
		{
			_FData->m_right = true;
			_Mov_Vec += {1, 0, 0};
		}

		if (m_KeyIO->Key_Press(VK_LEFT))
		{
			_FData->m_left = true;
			_Mov_Vec += {-1, 0, 0};
		}

		if (m_KeyIO->Key_Press(VK_UP))
		{
			_FData->m_forward = true;
			_Mov_Vec += {0, 0, -1};
		}

		if (m_KeyIO->Key_Press(VK_DOWN))
		{
			_FData->m_back = true;
			_Mov_Vec += {0, 0, 1};
		}
	}
	// 2p 조작
	else if (m_Player_Number == EATER_PLAYER_02)
	{
		if (m_KeyIO->Key_Press('D'))
		{
			_FData->m_right = true;
			_Mov_Vec += {1, 0, 0};
		}

		if (m_KeyIO->Key_Press('A'))
		{
			_FData->m_left = true;
			_Mov_Vec += {-1, 0, 0};
		}

		if (m_KeyIO->Key_Press('W'))
		{
			_FData->m_forward = true;
			_Mov_Vec += {0, 0, -1};
		}

		if (m_KeyIO->Key_Press('S'))
		{
			_FData->m_back = true;
			_Mov_Vec += {0, 0, 1};
		}
	}
	_Mov_Vec *= m_Current_Player->m_Speed;

	// 물리 처리를 위한 속도 설정.
	m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec.x, _Mov_Vec.y, _Mov_Vec.z);
	_FData->m_Mov_Vec3 = _Mov_Vec;

	auto _KeyIO_Data = Eater::GameData::CreateClientMove(_Builder, _FData->m_Sequence, _FData->m_forward, _FData->m_back, _FData->m_right, _FData->m_left, _FData->m_dash, _FData->m_skill1, _FData->m_skill2);

	// 데이터 직렬화 후 전송.
	_Builder.Finish(_KeyIO_Data);

	_Send_Packet->Packet_Type = C2S_PLAYER_MOVE;
	_Send_Packet->Packet_Size = _Builder.GetSize();
	memcpy_s(_Send_Packet->Packet_Buffer, _Builder.GetSize(), _Builder.GetBufferPointer(), _Builder.GetSize());

	m_Network->Send(_Send_Packet);
}

void GameManager::PhysX_World_Update()
{
	if (m_Is_Start == false)
	{
		return;
	}

	m_Current_Player->m_Position = m_Dest_Player->m_Position;

	// 정의된 dtime 을 기준으로하여 물리 연산 업데이트.
	PhysX_Update(PHYSX_UPDATE_TIME);

	// 업데이트 된 위치를 받아옴.
	PhysX_Update_Actor(m_Dest_Player->m_PhysX);

	m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;

	// 예측된 포지션 위치를 저장.
	_FData->m_Predict_Position = m_Dest_Player->m_Position;

	// 프레임데이터 저장.
	m_Client_Predict.push(_FData);
	_FData = nullptr;

	m_Player_List[m_Player_Number]->m_Sequence += 1;

	// 시퀀스값이 프레임을 나타내므로, uint로 바꿔서 누적해둔다. uint 최대값까지 게임을 할일이 있을까?.
	if (m_Player_List[m_Player_Number]->m_Sequence == MAX_SEQUENCE)
	{
		m_Player_List[m_Player_Number]->m_Sequence = 0;
	}

}

void GameManager::Finalize()
{
	MYD2D->DiscardGraphicsResources();
}

// 메시지 핸들러 (윈도 콜백)
LRESULT CALLBACK GameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
