#include "LoginManager.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"

LoginManager::LoginManager()
{
	InitializeCriticalSection(&g_CS);

	// �κ񼭹����� ������ȯ�� ���� ��Ʈ��ũ (Ŭ���̾�Ʈ)
	//m_Connect_Lobby = new DHNetWorkAPI();
	//m_Connect_Lobby->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	// ���� ���
	//while (m_Connect_Lobby->Connect(CONNET_PORT, CONNECT_IP)) { Sleep(0); };
	
	Is_Connect_Lobby = true;

	m_DB = new DHDB();
	m_DB->ConnectDB("221.163.91.100", "CDH", "ehxk2Rnfwoa!", "LOGIN", DATABASE_PORT);
	// �κ� ������ �����ϱ����� ������ ����
	m_Lobby_thread = new std::thread(std::bind(&LoginManager::LobbySideLogic, this));

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
