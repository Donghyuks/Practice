
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

	// ������ ��ü���� ������
	m_Timer = new DHTimer();
	m_Timer->SetFrame(5000);

	// Key Input Ÿ�̸�
	m_KeyIO_Timer = new DHTimer();
	m_KeyIO_Timer->SetFrame(60);

	_Send_Packet = new C2S_Packet();

	/// Win32 ����
	// ���� Ŭ����
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

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ���� ����
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

	// �÷��̾� ������.
	m_Dest_Player = new Player();
	m_Dest_Player->m_PhysX = new PhysData();
	m_Dest_Player->m_PhysX->SetLockAxis_Position(false, true, false);
	m_Dest_Player->m_PhysX->SetLockAxis_Rotation(true, true, true);


	// ���� ����..
	m_Network = new DHNetWorkAPI();
	m_Network->Initialize(DHNetWork_Name::DHNet);
	while (!m_Network->Connect(GAME_SERVER_PORT_1, LOCAL_CONNECT_IP)) {};

	// �׷��Ƚ� ���� �ʱ�ȭ
	MYD2D->Initialize(g_hwnd);

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(g_hwnd, SW_SHOWNORMAL);
	UpdateWindow(g_hwnd);

	MYD2D->SetGridMod(true, 100.0f, 100.0f, D2D1::ColorF(0.5f, 0.5f, 0.5f));

	// �ε��Ϸ�
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
		�⺻���� Ŭ���̾�Ʈ ���� ����.
			1.		�����κ��� ���¸� ������Ʈ �ް� üũ�Ѵ�.
			2-1.	Input�� �޴´�. (60FPS ���� ����)
			2-2.	PhysX World�� ������Ʈ �Ѵ�.
			3.		������ �����Ѵ�.
			4.		Render �Ѵ�.
	*/

	/// 1. �����κ��� ���¸� ������Ʈ �ް� üũ�Ѵ�.
	if (m_Network->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

			if (m_Is_Start == false)
			{
				// ó���� ���� ���° �������� �˷��ش�.
				if (Recv_Packet->Packet_Type == S2C_LOADING_COMPLETE_RES)
				{
					// ������ ĳ����.
					unsigned short* Recv_Player_Index = (unsigned short*)Recv_Packet->Packet_Buffer;

					// �������� �˷��� �ڽ��� �÷��̾� ��ȣ.
					m_Player_Number = *Recv_Player_Index;
				}

				// ���� ���۽� ������ ���� �޾ƿ��� ĳ���ͳ� �ӵ� ������.
				if (Recv_Packet->Packet_Type == S2C_START_GAME)
				{
					// ������ ĳ����.
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					const Eater::GameData::StartInfo* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::StartInfo>(Recv_Data_Ptr);

					auto _Players_Data = _Recv_Data->users();

					for (int i = 0; i < _Players_Data->size(); i++)
					{
						auto Player_Data = _Players_Data->Get(i);

						auto _Player_Index = Player_Data->playerindex();

						// ������ ���
						Player* _Player = new Player();
						_Player->m_PhysX = new PhysData();
						m_Player_List.insert({ _Player_Index , _Player });

						// ������ ���� �޾ƿ� ĳ���� ������ �ӵ�.
						m_Player_List[_Player_Index]->m_Character = Player_Data->character_type();
						m_Player_List[_Player_Index]->m_Speed = Player_Data->world_speed();
						m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(Player_Data->world_position()->x(),
							Player_Data->world_position()->y(), Player_Data->world_position()->z());

						m_Player_List[_Player_Index]->m_Prev_Position = m_Player_List[_Player_Index]->m_Position;

						// ���� ������ �������� ���
						if (_Player_Index == m_Player_Number)
						{
							m_Current_Player = m_Player_List[m_Player_Number];
							m_Dest_Player->m_PhysX->SetWorldPosition(m_Current_Player->m_Position.x, m_Current_Player->m_Position.y, m_Current_Player->m_Position.z);
							PhysX_Create_Actor(m_Dest_Player->m_PhysX);
						}
						/// �ӽ÷� ��������� ������ �޽��� ����س���
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
				// �������� �ֱ������� �������� World ����.
				if (Recv_Packet->Packet_Type == S2C_WORLD_UPDATE)
				{
					Enemy_List.clear();
					Mana_List.clear();

					Current_Time = std::chrono::system_clock::now();

					Passed_Time = Current_Time - Prev_Time;

					Prev_Time = Current_Time;

					// �����κ��� �����͸� �޾����Ƿ�, ���ð� �ʱ�ȭ.
					m_Passed_Client_Time = 0.f;

					// ������ ĳ����.
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					const Eater::GameData::WorldData* _Recv_Data = flatbuffers::GetRoot<Eater::GameData::WorldData>(Recv_Data_Ptr);

					// ���� ���� ����
					auto _Enemy_Data = _Recv_Data->enemies();
					for (int i = 0; i < _Enemy_Data->size(); i++)
					{
						auto _Enemy = _Enemy_Data->Get(i);
						auto _Position = _Enemy->position();

						Enemy_List.push_back({ _Enemy->index(), DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z()) });
					}

					// �ʿ� ������Ʈ (������ ����)
					auto _Object_Data = _Recv_Data->objects();
					for (int i = 0; i < _Object_Data->size(); i++)
					{
						auto _Object = _Object_Data->Get(i);
						auto _Position = _Object->position();

						Mana_List.push_back({ _Object->index(), DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z()) });
					}
					
					// ���������� ���� �κ�
					auto _Players_Data = _Recv_Data->users();

					for (int i = 0; i < _Players_Data->size(); i++)
					{
						auto Player_Data = _Players_Data->Get(i);

						// ���� ��ȣ.
						auto _Player_Index = Player_Data->playerindex();
						// �������� ó���� ������ ��ȣ
						auto _Sequence_Num = Player_Data->sequence();
						// Animation
						auto _Animation = Player_Data->animation();
						// Position
						auto _Position = Player_Data->position();
						// Mov_Vec
						auto _Mov_Vec = Player_Data->mov_vector();

						// ���� �÷������� ������
						if (_Player_Index == m_Player_Number)
						{
							while (!m_Client_Predict.empty())
							{
								// ť���� �����͸� ������� ����
								FrameData* _FData = m_Client_Predict.front();
								m_Client_Predict.pop();

								// ���� �ش� ���������� ó���� ���� �����Ͱ� �����κ��� �Ѿ����, �ش絥���Ͱ� ���������� ��� ��� �����Ѵ�.
								if (_Sequence_Num == _FData->m_Sequence)
								{
									// �������� �˻�
									if (abs(_Position->x() - _FData->m_Predict_Position.x) > m_Epsilon ||
										abs(_Position->z() - _FData->m_Predict_Position.z) > m_Epsilon)
									{
										// ������ ���� ���� �����ͷ� ��� �� ���¸� �� �ùķ��̼� �Ѵ�.
										m_Dest_Player->m_PhysX->SetWorldPosition(_Position->x(), _Position->y(), _Position->z());
										m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;
										m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec->x(), _Mov_Vec->y(), _Mov_Vec->z());

										// ���ǵ� dtime �� ���������Ͽ� ���� ���� ������Ʈ.
										//PhysX_Update(PHYSX_UPDATE_TIME);
										//PhysX_Update_Actor(m_Dest_Player->m_PhysX);

										m_Client_BackUp = m_Client_Predict;

										while (!m_Client_BackUp.empty())
										{
											// ť���� �����͸� ������� ����
											FrameData* _BackUp_FData = m_Client_BackUp.front();
											m_Client_BackUp.pop();

											// ��� �÷��̾��� �ӵ��� �ְ�, ���� �ùķ��̼��� �����Ͽ�, �������� �����Ͱ��� �����Ѵ�.
											m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec->x() + _BackUp_FData->m_Mov_Vec3.x, 
												_BackUp_FData->m_Mov_Vec3.y + _Mov_Vec->y(),
												_BackUp_FData->m_Mov_Vec3.z + _Mov_Vec->z());
											
											// ���ǵ� dtime �� ���������Ͽ� ���� ���� ������Ʈ.
											PhysX_Update(PHYSX_UPDATE_TIME);
											PhysX_Update_Actor(m_Dest_Player->m_PhysX);

											m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;
											// ���� ��ġ ����.
											_BackUp_FData->m_Predict_Position = m_Dest_Player->m_Position;
										}

										// ����� ������ ����
										delete _FData;
										_FData = nullptr;
										break;
									}
								}

								// ����� ������ ����
								delete _FData;
								_FData = nullptr;
							}

						}
						// �ٸ� Ŭ���̾�Ʈ �����ʹ� �������� �޾ƿ� �����Ͱ��� ����.
						else
						{
							// ���� �����ǰ� ��� (����������)
							m_Player_List[_Player_Index]->m_Prev_Position = m_Player_List[_Player_Index]->m_Position;
							m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z());
							m_Player_List[_Player_Index]->m_Animation = _Animation;
							m_Player_List[m_Player_Number]->m_PhysX->SetWorldPosition(_Position->x(), _Position->y(), _Position->z());
							m_Player_List[m_Player_Number]->m_PhysX->SetVelocity(_Mov_Vec->x(), _Mov_Vec->y(), _Mov_Vec->z());
						}
					}
				}
			}

			// ����ѵ����� ����.
			delete S2C_Msg.Packet;
			S2C_Msg.Packet = nullptr;
		}
		Msg_Vec.clear();
	}

	// ������ ���۵��� �ʾҴٸ� �ƹ��͵� ��������..
	if (m_Is_Start == false) return;


	/// 3,4 ������ ������ �� ������.
	MYD2D->StartRender();
	MYD2D->thDrawText(10, 10, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Current FPS : %.2f Fps", (double)1 / m_Dtime );
	MYD2D->thDrawText(10, 30, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Real RTT : %.2f s", Passed_Time.count());
	MYD2D->thDrawText(10, 50, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Setting RTT : %.2f s", m_RTT );
	MYD2D->thDrawText(10, 70, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Passed Client Time : %.2f s", m_Passed_Client_Time);

	float Scaleing = 10.f;

	// �������� �׸���
	for (auto _Mana_Stone : Mana_List)
	{
		// 2D �� ���� ���̱����� ����
		DirectX::SimpleMath::Vector3 _ADD_Pos(100, 0, 250);
		DirectX::SimpleMath::Vector3 _Draw_Pos = (_Mana_Stone.Position * Scaleing) + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Mana_Stone.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z , 1 * Scaleing, 1, D2D1::ColorF(0.0f, 0.0f, 1.0f));
	}

	// �� �׸���
	for (auto _Enemy : Enemy_List)
	{
		// 2D �� ���� ���̱����� ����
		DirectX::SimpleMath::Vector3 _ADD_Pos(100, 0, 250);
		DirectX::SimpleMath::Vector3 _Draw_Pos = (_Enemy.Position * Scaleing) + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Enemy.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z, 1 * Scaleing, 1, D2D1::ColorF(1.0f, 0.0f, 1.0f));
	}

	// ������ ������ ������
	DirectX::SimpleMath::Vector3 _Interpolation_Pos;

	// ������� (������ ��Žð� (20������) ���� ���� Ŭ���̾�Ʈ�� ��Ʈ��ũ�κ��� �����͸� ������ ���� ���� �ð��� ������ )
	double _Interpolation = m_Passed_Client_Time / m_RTT;
	if (_Interpolation >= 1.0f) _Interpolation = 1.0f;

	/// ��ǥ�� �����ֱ� ���ϱ����ؼ� ��ǥ����.
	DirectX::SimpleMath::Vector3 _Offset_Vec(100, 0, 250);

	/// ������� (����)

	for (auto _Other : m_Player_List)
	{
		// ���� ���� ������ �ǳʶ�..
		if (_Other.second == m_Current_Player) continue;

		Player* _Other_Player = _Other.second;

		_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(_Other_Player->m_Prev_Position, _Other_Player->m_Position, _Interpolation);
		_Interpolation_Pos *= Scaleing;
		_Interpolation_Pos += _Offset_Vec;

		if (_Other_Player->m_Animation == ANIMATION_IDLE)
		{
			// ��� ������ ������ ������ ������ �� �����϶� ������ �׸�
			MYD2D->thDrawEllipse(_Interpolation_Pos.x, _Interpolation_Pos.z, 1 * Scaleing, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
		else if (_Other_Player->m_Animation == ANIMATION_RUN)
		{
			// ��� ������ ������ ������ ������ �� �����϶� ������ �׸�
			MYD2D->thDrawRectangle(_Interpolation_Pos.x - 4, _Interpolation_Pos.z - 4,
				_Interpolation_Pos.x + 4, _Interpolation_Pos.z + 4, 0.f, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
	}

	/// �������� (����)

	_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(m_Current_Player->m_Position, m_Dest_Player->m_Position, _Interpolation);
	_Interpolation_Pos *= Scaleing;
	_Interpolation_Pos += _Offset_Vec;

	if (m_Current_Player->m_Animation == ANIMATION_IDLE)
	{
		// ������ �ִ� ��� �ʷϻ� ��
		MYD2D->thDrawEllipse(_Interpolation_Pos.x, _Interpolation_Pos.z, 1 * Scaleing, 2, D2D1::ColorF(0.0f, 1.0f, 0.0f));
	}
	else if (m_Current_Player->m_Animation == ANIMATION_RUN)
	{
		// �����̴� ��� ������ �׸�
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

	/// 2. Input�� �޴´�. ( ���� Input�� ��Ʈ��ũ�� �޼��� ���� )
	// ������ Input�� ������ ���� ����
	flatbuffers::FlatBufferBuilder _Builder;

	// �̹� �����ӿ� ���� ������ ���� �� ť��.
	_FData = new FrameData();
	_FData->m_Sequence = m_Player_List[m_Player_Number]->m_Sequence;

	/// Ű �Է��� ����� �д�.
	DirectX::SimpleMath::Vector3 _Mov_Vec = DirectX::SimpleMath::Vector3(0,0,0);
	// 1p ����
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
	// 2p ����
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

	// ���� ó���� ���� �ӵ� ����.
	m_Dest_Player->m_PhysX->SetVelocity(_Mov_Vec.x, _Mov_Vec.y, _Mov_Vec.z);
	_FData->m_Mov_Vec3 = _Mov_Vec;

	auto _KeyIO_Data = Eater::GameData::CreateClientMove(_Builder, _FData->m_Sequence, _FData->m_forward, _FData->m_back, _FData->m_right, _FData->m_left, _FData->m_dash, _FData->m_skill1, _FData->m_skill2);

	// ������ ����ȭ �� ����.
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

	// ���ǵ� dtime �� ���������Ͽ� ���� ���� ������Ʈ.
	PhysX_Update(PHYSX_UPDATE_TIME);

	// ������Ʈ �� ��ġ�� �޾ƿ�.
	PhysX_Update_Actor(m_Dest_Player->m_PhysX);

	m_Dest_Player->m_Position = m_Dest_Player->m_PhysX->WorldPosition;

	// ������ ������ ��ġ�� ����.
	_FData->m_Predict_Position = m_Dest_Player->m_Position;

	// �����ӵ����� ����.
	m_Client_Predict.push(_FData);
	_FData = nullptr;

	m_Player_List[m_Player_Number]->m_Sequence += 1;

	// ���������� �������� ��Ÿ���Ƿ�, uint�� �ٲ㼭 �����صд�. uint �ִ밪���� ������ ������ ������?.
	if (m_Player_List[m_Player_Number]->m_Sequence == MAX_SEQUENCE)
	{
		m_Player_List[m_Player_Number]->m_Sequence = 0;
	}

}

void GameManager::Finalize()
{
	MYD2D->DiscardGraphicsResources();
}

// �޽��� �ڵ鷯 (���� �ݹ�)
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
