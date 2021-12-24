#include "LoginManager.h"
#include "DHNetWorkAPI.h"
#include "DHDB.h"
#include "DHTimer.h"

LoginManager::LoginManager()
{
	InitializeCriticalSection(&g_CS);

	m_Timer = new DHTimer;
	m_Timer->SetTime(5);

	// 로비서버와의 정보교환을 위한 네트워크 (클라이언트)
	//m_Connect_Lobby = new DHNetWorkAPI();
	//m_Connect_Lobby->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);
	// 연결 대기
	//while (m_Connect_Lobby->Connect(CONNET_PORT, CONNECT_IP)) { Sleep(0); };
	
	Is_Connect_Lobby = true;

	m_DB = new DHDB();
	m_DB->ConnectDB("221.163.91.100", "CDH", "ehxk2Rnfwoa!", "LOGIN", DATABASE_PORT);
	// 로비 로직을 실행하기위한 쓰레드 생성
	m_Lobby_Thread = new std::thread(std::bind(&LoginManager::LobbySideLogic, this));

	// 일정주기로 접속되어있는 런쳐에게 정보를 보내고, 접속을 체크하는 로직
	m_Keep_Alive_Thread = new std::thread(std::bind(&LoginManager::KeepAlive, this));

	// 게임런처와의 정보교환을 위한 네트워크 (서버)
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
				// 클라이언트의 소켓 번호
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// 클라이언트가 보낸 메세지
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);
				// 클라이언트가 강제종료한 경우
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (C2S_Msg->Packet_Type == C2S_LOGIN_SAFE_REQ)
				{
					// 로그인 인증절차.
					Login_Certify(_Recv_Socket_Num, C2S_Msg);
				}

				// 사용한데이터 해제.
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

		// 설정한시간 이 되었을경우
		if (m_Timer->Ready_Frame())
		{
			EnterCriticalSection(&g_CS);
			// 한번이라도 접속한 유저들에 대해서 친구정보를 업데이트 해준다.
			for (auto& _User_Data : Logged_In_User_Data)
			{
				// 유저의 친구들에 대해서 상태를 체크한다.
				for (const auto _User_Friend : _User_Data.second->Friend_List)
				{
					auto _Friend_Name = KeepAlive_Builder.CreateString(_User_Friend);

					// 친구들이 접속한적이 있는지 검사
					auto _Friend_Data = Logged_In_User_Data.find(_User_Friend);

					// 만약 친구가 접속한적이 없는경우
					if (_Friend_Data == Logged_In_User_Data.end())
					{
						// 해당 친구는 오프라인으로 체크해서 친구목록에 등록한다.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, USER_OFFLINE, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
					// 한번이라도 접속한 친구인 경우
					else
					{
						// 현재 상태로 유저의 상태로 친구목록에 등록한다.
						auto _Friend_State = Eater::LoginLauncher::CreateUserState(KeepAlive_Builder, _Friend_Data->second->User_State, _Friend_Name);
						_friend_state_list.push_back(_Friend_State);
					}
				}

				// 데이터를 정리해서 런처에 보내준다.
				auto Send_RealTimeData = CreateRealTimeDataDirect(KeepAlive_Builder, &_friend_state_list, &_friend_request_list);

				KeepAlive_Builder.Finish(Send_RealTimeData);

				KeepAlive_Send_Packet.Packet_Type = S2C_KEEP_ALIVE_CHECK_REQ;
				KeepAlive_Send_Packet.Packet_Size = KeepAlive_Builder.GetSize();
				memcpy_s(KeepAlive_Send_Packet.Packet_Buffer, KeepAlive_Builder.GetSize(), KeepAlive_Builder.GetBufferPointer(), KeepAlive_Builder.GetSize());

				// Send 실패시 연결이 끊켰으므로 유저의 정보를 업데이트해준다.
				if (m_Connect_Launcher->Send(&KeepAlive_Send_Packet, SEND_TYPE_TARGET, _User_Data.second->Socket_Num) == false)
				{
					_User_Data.second->Socket_Num = INVALID_SOCKET;
					_User_Data.second->User_State = USER_OFFLINE;
				}		

				// 사용한 데이터 재사용을 위한 정리.
				KeepAlive_Builder.Clear();
				_friend_state_list.clear();
				_friend_request_list.clear();
			}

			// 추후 친구요청 데이터 구현시, 이부분에서 추가해서 같이 보내준다.
			//...
			LeaveCriticalSection(&g_CS);

		}

		Sleep(0);
	}
}

void LoginManager::Login_Certify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg)
{
	// 로그인 성공여부.
	bool Is_Login_Success = false;

	// 받은 데이터를 flatbuffer 에 맞게 캐스팅
	const uint8_t* Recv_Data_Ptr = (unsigned char*)_C2S_Msg->Packet_Buffer;
	const auto Recv_LoginReqData = flatbuffers::GetRoot<Eater::LoginLauncher::LoginReqData>(Recv_Data_Ptr);

	// 클라이언트에서 로그인 시도한 아이디와 패스워드.
	auto Login_ID = Recv_LoginReqData->id()->str();
	auto Login_Password = Recv_LoginReqData->password()->str();

	// 서버에서 클라이언트에게 보낼 메세지 타입
	Launcher_Send_Packet.Packet_Type = S2C_LOGIN_SAFE_RES;
	flatbuffers::Offset<Eater::LoginLauncher::LoginResData> Send_LoginResData;

	if (m_DB->SearchID(Login_ID))
	{
		// 아이디 O 패스워드 O (success)
		if (m_DB->ComparePassword(Login_ID, Login_Password))
		{
			// 보낼 데이터를 만든다. ( 로그인 결과와 해당하는 아이디에 대한 식별값을 같이 보내준다. )
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_SUCCESS, m_DB->GetIdentifier(Login_ID));
			Is_Login_Success = true;	// 로그인성공
		}
		// 아이디 O 패스워드 X
		else
		{
			Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_PASSWORD_FAIL, m_DB->GetIdentifier(Login_ID));
		}
	}
	// 아이디 X 패드워드 X
	else
	{
		Send_LoginResData = Eater::LoginLauncher::CreateLoginResData(Launcher_Builder, LOGIN_ID_FAIL, m_DB->GetIdentifier(Login_ID));
	}

	// 데이터 직렬화
	Launcher_Builder.Finish(Send_LoginResData);
	Launcher_Send_Packet.Packet_Size = Launcher_Builder.GetSize();
	memcpy_s(Launcher_Send_Packet.Packet_Buffer, Launcher_Builder.GetSize(), Launcher_Builder.GetBufferPointer(), Launcher_Builder.GetSize());

	// 해당 패킷을 보냄.
	m_Connect_Launcher->Send(&Launcher_Send_Packet, SEND_TYPE_TARGET, _Socket_Num);

	Launcher_Builder.Clear();

	// 로그인이 성공한다면 로그인서버 정보에 등록한다.
	if (Is_Login_Success)
	{
		auto User_Init = Logged_In_User_Data.find(Login_ID);
		// 존재하지 않는 유저라면
		if (User_Init == Logged_In_User_Data.end())
		{
			// 유저 의 아이디와 소켓번호 그리고 친구목록을 등록한다.
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
			// 이미 존재하는 유저라면 (해당 유저의 소켓정보를 업데이트 해준다.)
			User_Init->second->Socket_Num.exchange(_Socket_Num);
		}

	}
}
