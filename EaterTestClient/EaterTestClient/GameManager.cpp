
#include "GameManager.h"
#include "DHKeyIO.h"
#include "DHTimer.h"
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "WorldData_generated.h"
#include "PortIPDefine.h"
#include "math.h"

HRESULT GameManager::Initialize(HINSTANCE hInstance)
{
	Prev_Time = std::chrono::system_clock::now();
	Current_Time = std::chrono::system_clock::now();
	m_KeyIO = new DHKeyIO();
	m_RTT = (double)1.f / (double)SERVER_FREQUENCY;

	m_Timer = new DHTimer();
	m_Timer->SetFrame(5000);

	_Send_Packet = new C2S_Packet();

	/// �ӽ� �÷��̾� ������.
	Player* _Player01 = new Player();
	m_Player_List.insert({ EATER_PLAYER_01 , _Player01 });

	Player* _Player02 = new Player();
	m_Player_List.insert({ EATER_PLAYER_02 , _Player02 });

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

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			DispatchMessage(&msg);
		}
		else
		{
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
			1. �����κ��� ���¸� ������Ʈ �ް� üũ�Ѵ�.
			2. Input�� �޴´�. ( ���� Input�� ��Ʈ��ũ�� �޼��� ���� )
			3. ������ �����Ѵ�.
			4. Render �Ѵ�.
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
					m_Player = m_Player_List[m_Player_Number];
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

						// ������ ���� �޾ƿ� ĳ���� ������ �ӵ�.
						m_Player_List[_Player_Index]->m_Character = Player_Data->character_type();
						m_Player_List[_Player_Index]->m_Speed = Player_Data->world_speed();
						m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(Player_Data->world_position()->x(),
							Player_Data->world_position()->y(), Player_Data->world_position()->z());
						m_Player_List[_Player_Index]->m_Prev_Position = m_Player_List[_Player_Index]->m_Position;
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
									if (abs(_Position->x() - _FData->m_Position.x) > m_Epsilon ||
										abs(_Position->y() - _FData->m_Position.y) > m_Epsilon ||
										abs(_Position->z() - _FData->m_Position.z) > m_Epsilon)
									{
										// ������ ���� ���� �����ͷ� ���.
										m_Player_List[_Player_Index]->m_Position = DirectX::SimpleMath::Vector3(_Position->x(), _Position->y(), _Position->z());
										m_Player_List[_Player_Index]->m_Animation = _Animation;

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

	/// 2. Input�� �޴´�. ( ���� Input�� ��Ʈ��ũ�� �޼��� ���� )
	// Ű �Է��� ����ؼ� ������ ���� ���͸� ���Ѵ�.
	// 1p ����
	if (m_Player_Number == EATER_PLAYER_01)
	{
		if (m_KeyIO->Key_Press(VK_RIGHT))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(1.0f, 0.f, 0.f);
		}

		if (m_KeyIO->Key_Press(VK_LEFT))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(-1.0f, 0.f, 0.f);
		}

		if (m_KeyIO->Key_Press(VK_UP))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(0.f, -1.f, 0.f);
		}

		if (m_KeyIO->Key_Press(VK_DOWN))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f);
		}
	}
	// 2p ����
	else if (m_Player_Number == EATER_PLAYER_02)
	{
		if (m_KeyIO->Key_Press('D'))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(1.0f, 0.f, 0.f);
		}

		if (m_KeyIO->Key_Press('A'))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(-1.0f, 0.f, 0.f);
		}

		if (m_KeyIO->Key_Press('W'))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(0.f, -1.f, 0.f);
		}

		if (m_KeyIO->Key_Press('S'))
		{
			m_Client_Move_Vector += DirectX::SimpleMath::Vector3(0.f, 1.f, 0.f);
		}
	}

	// �̹������ӿ� �������� �ִ°��
	if (m_Client_Move_Vector != DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f))
	{
		// ��Ʈ��ũ�� ��ǲ�� ����
		flatbuffers::FlatBufferBuilder _Builder;

		auto _Mov_Vec = Eater::GameData::Vec3(m_Client_Move_Vector.x, m_Client_Move_Vector.y, m_Client_Move_Vector.z);
		unsigned short _Current_Sequence = m_Player_List[m_Player_Number]->m_Sequence++;

		// �̹� �����ӿ� ���� ������ ���� �� ť��.
		FrameData* _FData = new FrameData();
		_FData->m_Sequence = _Current_Sequence;
		_FData->m_Mov_Vector = m_Client_Move_Vector;
		_FData->m_dtime = m_Dtime;

		// �ִ밪�� �����ϸ� �ٽ� 0���� �����ϵ���.
		if (_Current_Sequence == MAX_SEQUENCE)
		{
			m_Player_List[m_Player_Number]->m_Sequence = 0;
		}

		auto _Move_Packet_Data = Eater::GameData::CreateClientMove(_Builder, _Current_Sequence, m_Dtime, &_Mov_Vec);

		// ������ ����ȭ �� ����.
		_Builder.Finish(_Move_Packet_Data);

		_Send_Packet->Packet_Type = C2S_PLAYER_MOVE;
		_Send_Packet->Packet_Size = _Builder.GetSize();
		memcpy_s(_Send_Packet->Packet_Buffer, _Builder.GetSize(), _Builder.GetBufferPointer(), _Builder.GetSize());

		m_Network->Send(_Send_Packet);

		m_Player->m_Animation = ANIMATION_RUN;


		// ���� �÷������� �÷��̾�� ������ ����� �ڿ������� Ű��ǲ��� ������Ʈ.
		//////////////////////////////////////////////////////////////////////////
		//				�ݵ�� ������ �̵� ���� ������ �Ȱ��ƾ� �� ��				//
		//////////////////////////////////////////////////////////////////////////
		m_Player->m_Position += (m_Player->m_Speed * m_Dtime * m_Client_Move_Vector);
		_FData->m_Position = m_Player_List[m_Player_Number]->m_Position;

		// �����ӵ����� ����.
		m_Client_Predict.push(_FData);
	}
	// �������� ���°��
	else
	{
		m_Player->m_Animation = ANIMATION_IDLE;
	}

	/// 3,4 ������ ������ �� ������.
	MYD2D->StartRender();
	MYD2D->thDrawText(10, 10, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Current FPS : %.2f Fps", (double)1 / m_Dtime );
	MYD2D->thDrawText(10, 30, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Real RTT : %.2f s", Passed_Time.count());
	MYD2D->thDrawText(10, 50, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Setting RTT : %.2f s", m_RTT );
	MYD2D->thDrawText(10, 70, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Passed Client Time : %.2f s", m_Passed_Client_Time);

	// �������� �׸���
	for (auto _Mana_Stone : Mana_List)
	{
		// 2D �� ���� ���̱����� ����
		DirectX::SimpleMath::Vector3 _ADD_Pos(300, 0, 500);
		DirectX::SimpleMath::Vector3 _Draw_Pos = _Mana_Stone.Position * 3 + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Mana_Stone.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z, 7, 1, D2D1::ColorF(0.0f, 0.0f, 1.0f));
	}

	// �� �׸���
	for (auto _Enemy : Enemy_List)
	{
		// 2D �� ���� ���̱����� ����
		DirectX::SimpleMath::Vector3 _ADD_Pos(300, 0, 500);
		DirectX::SimpleMath::Vector3 _Draw_Pos = _Enemy.Position * 3 + _ADD_Pos;
		MYD2D->thDrawText(_Draw_Pos.x, _Draw_Pos.z, D2D1::ColorF(0.0f, 0.0f, 0.0f), L"%d", _Enemy.Index);
		MYD2D->thDrawEllipse(_Draw_Pos.x, _Draw_Pos.z, 7, 1, D2D1::ColorF(1.0f, 0.0f, 1.0f));
	}

	// ���� �÷��̾ ������ ��������� RTT �� ����Ͽ� ����
	for (auto _Other : m_Player_List)
	{
		// �ٸ� ������ ������ (���� ������)
		DirectX::SimpleMath::Vector3 _Interpolation_Pos;
		// ���� ���� ������ �ǳʶ�..
		if (_Other.second == m_Player) continue;

		Player* _Other_Player = _Other.second;

		// ������� (������ ��Žð� (20������) ���� ���� Ŭ���̾�Ʈ�� ��Ʈ��ũ�κ��� �����͸� ������ ���� ���� �ð��� ������ )
		double _Interpolation = m_Passed_Client_Time / m_RTT;
		if (_Interpolation >= 1.0f) _Interpolation = 1.0f;

		_Interpolation_Pos = DirectX::SimpleMath::Vector3::Lerp(_Other_Player->m_Prev_Position, _Other_Player->m_Position, _Interpolation);

		if (_Other_Player->m_Animation == ANIMATION_IDLE)
		{
			// ��� ������ ������ ������ ������ �� �����϶� ������ �׸�
			MYD2D->thDrawEllipse(_Interpolation_Pos.x, _Interpolation_Pos.y, 20, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
		else if (_Other_Player->m_Animation == ANIMATION_RUN)
		{
			// ��� ������ ������ ������ ������ �� �����϶� ������ �׸�
			MYD2D->thDrawRectangle(_Interpolation_Pos.x - 20, _Interpolation_Pos.y - 20,
				_Interpolation_Pos.x + 20, _Interpolation_Pos.y + 20, 0.f, 2, D2D1::ColorF(1.0f, 0.0f, 0.0f));
		}
	}

	if (m_Player->m_Animation == ANIMATION_IDLE)
	{
		// ���� ������ ������ ������ �ʷϻ� �� �����϶� �ʷϻ� �׸�
		MYD2D->thDrawEllipse(m_Player->m_Position.x, m_Player->m_Position.y, 20, 2, D2D1::ColorF(0.0f, 1.0f, 0.0f));
	}
	else if (m_Player->m_Animation == ANIMATION_RUN)
	{
		// ��� ������ ������ ������ ������ �� �����϶� ������ �׸�
		MYD2D->thDrawRectangle(m_Player->m_Position.x - 20, m_Player->m_Position.y - 20,
			m_Player->m_Position.x + 20, m_Player->m_Position.y + 20, 0.f, 2, D2D1::ColorF(0.0f, 1.0f, 0.0f));
	}

	MYD2D->EndRender();
}

void GameManager::TestLoop()
{
	MYD2D->StartRender();
	MYD2D->thDrawText(10, 10, D2D1::ColorF(1.0f, 0.0f, 0.0f), L"Current FPS : %.6f ms", m_Dtime);
	MYD2D->EndRender();
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
