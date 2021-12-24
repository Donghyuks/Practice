#include "LoginManager.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"
#include "DHTimer.h"

LoginManager::LoginManager()
{
	InitializeCriticalSection(&g_CS);

	m_Timer = new DHTimer;
	m_Timer->SetTime(5);

	// �κ񼭹����� ������ȯ�� ���� ��Ʈ��ũ (Ŭ���̾�Ʈ)
	//m_Connect_Lobby = new DHNetWorkAPI();
	//m_Connect_Lobby->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	// ���� ���
	//while (m_Connect_Lobby->Connect(CONNET_PORT, CONNECT_IP)) { Sleep(0); };
	
	Is_Connect_Lobby = true;

	m_DB = new DHDB();
	m_DB->ConnectDB("221.163.91.100", "CDH", "ehxk2Rnfwoa!", "LOGIN", DATABASE_PORT);
	// �κ� ������ �����ϱ����� ������ ����
	m_Lobby_Thread = new std::thread(std::bind(&LoginManager::LobbySideLogic, this));

	// �����ֱ�� ���ӵǾ��ִ� ���Ŀ��� ������ ������, ������ üũ�ϴ� ����
	m_Keep_Alive_Thread = new std::thread(std::bind(&LoginManager::KeepAlive, this));

	// ���ӷ�ó���� ������ȯ�� ���� ��Ʈ��ũ (����)
	m_Connect_Launcher = new DHNetWorkAPI();
	m_Connect_Launcher->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	m_Connect_Launcher->Accept(LOGIN_SERVER_PORT, 100, 2);
}

LoginManager::~LoginManager()
{

}

void LoginManager::LauncherSideLogic()
{
	while (Is_Connect_Lobby.load())
	{
		if (m_Connect_Launcher->Recv(Launcher_Msg_Vec))
		{
			for (auto& Msg_Packet : Launcher_Msg_Vec)
			{
				// Ŭ���̾�Ʈ�� ���� ��ȣ
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// Ŭ���̾�Ʈ�� ���� �޼���
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);
				// Ŭ���̾�Ʈ�� ���������� ���
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (C2S_Msg->Packet_Type == C2S_LOGIN_SAFE_REQ)
				{
					// �α��� ��������.
					Login_Certify(_Recv_Socket_Num, C2S_Msg);
				}

				// ����ѵ����� ����.
				delete Msg_Packet.Packet;
				Msg_Packet.Packet = nullptr;
			}
			Launcher_Msg_Vec.clear();
		}

		Sleep(0);
	}
}

void LoginManager::LobbySideLogic()
{
	while (true)
	{
		Sleep(0);
	}
}

void LoginManager::KeepAlive()
{
	std::vector<flatbuffers::Offset<Eater::LoginLauncher::UserState>> _friend_state_list;
	std::vector<flatbuffers::Offset<flatbuffers::String>> _friend_request_list;

	while (true)
	{
		m_Timer->Update();

		// �����ѽð� �� �Ǿ������
		if (m_Timer->Ready_Frame())
		{
			EnterCriticalSection(&g_CS);
			// �ѹ��̶� ������ �����鿡 ���ؼ� ģ�������� ������Ʈ ���ش�.
			for (auto& _User_Data : Logged_In_User_Data)
			{
				// ������ ģ���鿡 ���ؼ� ���¸� üũ�Ѵ�.
				for (const auto _User_Friend : _User_Data.second->Friend_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend);

					// ģ������ ���������� �ִ��� �˻�
					auto _Friend_Data = Logged_In_User_Data.find(_User_Friend);

					// ���� ģ���� ���������� ���°��
					if (_Friend_Data == Logged_In_User_Data.end())
					{
						// �ش� ģ���� ������������ üũ�ؼ� ģ����Ͽ� ����Ѵ�.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, USER_OFFLINE, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
					// �ѹ��̶� ������ ģ���� ���
					else
					{
						// ���� ���·� ������ ���·� ģ����Ͽ� ����Ѵ�.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, _Friend_Data->second->User_State, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
				}

				// �����͸� �����ؼ� ��ó�� �����ش�.
				auto Send_RealTimeData = CreateRealTimeDataDirect(KeepAlive_Builder, &_friend_state_list, &_friend_request_list);

				KeepAlive_Builder.Finish(Send_RealTimeData);

				KeepAlive_Send_Packet.Packet_Type = S2C_KEEP_ALIVE_CHECK_REQ;
				KeepAlive_Send_Packet.Packet_Size = KeepAlive_Builder.GetSize();
				memcpy_s(KeepAlive_Send_Packet.Packet_Buffer, KeepAlive_Builder.GetSize(), KeepAlive_Builder.GetBufferPointer(), KeepAlive_Builder.GetSize());

				// Send ���н� ������ �������Ƿ� ������ ������ ������Ʈ���ش�.
				if (m_Connect_Launcher->Send(&KeepAlive_Send_Packet, SEND_TYPE_TARGET, _User_Data.second->Socket_Num) == false)
				{
					_User_Data.second->Socket_Num = INVALID_SOCKET;
					_User_Data.second->User_State = USER_OFFLINE;
				}		

				// ����� ������ ������ ���� ����.
				KeepAlive_Builder.Clear();
				_friend_state_list.clear();
				_friend_request_list.clear();
			}

			// ���� ģ����û ������ ������, �̺κп��� �߰��ؼ� ���� �����ش�.
			//...
			LeaveCriticalSection(&g_CS);

		}

		Sleep(0);
	}
}

void LoginManager::Login_Certify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// �α��� ��������.
	bool Is_Login_Success = false;

	// ���� �����͸� flatbuffer �� �°� ĳ����
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_LoginReqData = flatbuffers::GetRoot<Eater::LoginLauncher::LoginReqData>(Recv_Data_Ptr);

	// Ŭ���̾�Ʈ���� �α��� �õ��� ���̵�� �н�����.
	auto Login_ID = Recv_LoginReqData->id()->str();
	auto Login_Password = Recv_LoginReqData->password()->str();

	// �������� Ŭ���̾�Ʈ���� ���� �޼��� Ÿ��
	Launcher_Send_Packet.Packet_Type = S2C_LOGIN_SAFE_RES;
	flatbuffers::Offset<Eater::LoginLauncher::LoginResData> Send_LoginResData;

	if (m_DB->SearchID(Login_ID))
	{
		// ���̵� O �н����� O (success)
		if (m_DB->ComparePassword(Login_ID, Login_Password))
		{
			// ���� �����͸� �����. ( �α��� ����� �ش��ϴ� ���̵� ���� �ĺ����� ���� �����ش�. )
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_SUCCESS, m_DB->GetIdentifier(Login_ID));
			Is_Login_Success = true;	// �α��μ���
		}
		// ���̵� O �н����� X
		else
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_PASSWORD_FAIL, m_DB->GetIdentifier(Login_ID));
		}
	}
	// ���̵� X �е���� X
	else
	{
		Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ID_FAIL, m_DB->GetIdentifier(Login_ID));
	}

	// ������ ����ȭ
	Launcher_Builder.Finish(Send_LoginResData);
	Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
	memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

	// �ش� ��Ŷ�� ����.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

	Launcher_Builder.Clear();

	// �α����� �����Ѵٸ� �α��μ��� ������ ����Ѵ�.
	if (Is_Login_Success)
	{
		auto User_Init = Logged_In_User_Data.find(Login_ID);
		// �������� �ʴ� �������
		if (User_Init == Logged_In_User_Data.end())
		{
			// ���� �� ���̵�� ���Ϲ�ȣ �׸��� ģ������� ����Ѵ�.
			User_Data* _User_Data = new User_Data;
			_User_Data->Socket_Num.exchange(_Socket_Num);
			_User_Data->User_State.exchange(USER_ONLINE);
			m_DB->GetFriendList(Login_ID, _User_Data->Friend_List);

			EnterCriticalSection(&g_CS);
			Logged_In_User_Data.insert({ Login_ID, _User_Data });
			LeaveCriticalSection(&g_CS);
		}
		else
		{
			// �̹� �����ϴ� ������� (�ش� ������ ���������� ������Ʈ ���ش�.)
			User_Init->second->Socket_Num.exchange(_Socket_Num);
		}

	}
}
