#pragma once

/*
	2021/12/23 15:58 - CDH
	
	< 생각정리 >
		1. 유저가 들어오면 들어오는 유저에 대한 정보 테이블(아이디 , 소켓번호) 을 제작한다.
		2. 들어온 유저에 대해서 친구 목록을 DB에서 조회하여 같이 등록한다.
		3. 해당 유저가 나간다면 소켓번호를 invalid 소켓으로 바꾼다.
		4. 일정시간동안 해당 테이블 정보를 유지한다. (추후 구현)
*/


#include <atomic>
#include <iostream>
#include "PortIPDefine.h"
#include "SharedDataStruct.h"
#include "LauncherLoginPacketDefine.h"
#include "LoginLauncher_generated.h"

class DHNetWorkAPI;
class DHDB;
class DHTimer;

struct User_Data
{
	// Socket번호가 INVAILD 면 현재 Offline 이라는 뜻이다.
	std::atomic<unsigned int> Socket_Num = INVALID_SOCKET;
	std::atomic<unsigned int> User_State = USER_OFFLINE;
	std::vector<std::string> Friend_List;
	std::vector<std::string> Friend_Request_List;	// 추후구현..
};

class LoginManager
{
private:
	CRITICAL_SECTION g_CS;

private:
	/// 네트워크 연결 및 로직 실행
	// 게임 런처와 연결할 네트워크 (서버로 생성)
	DHNetWorkAPI* m_Connect_Launcher = nullptr;
	// 로비 서버와 연결할 네트워크 (클라이언트로 생성)
	DHNetWorkAPI* m_Connect_Lobby = nullptr;
	// 로비 쪽과의 로직을 돌리기 위한 쓰레드
	std::thread* m_Lobby_Thread = nullptr;
	// 일정 주기로 로직을 처리하기 위한 쓰레드
	std::thread* m_Keep_Alive_Thread = nullptr;

	/// DB 연결
	DHDB* m_DB = nullptr;

	/// Timer
	DHTimer* m_Timer = nullptr;

	/// 로직에 필요한 변수
	// 로비랑 연결되었는지? (로비랑 연결이 안되어있다면 로그인서버는 동작하면 안된다.)
	std::atomic<bool> Is_Connect_Lobby = false;
	// Recv 버퍼 비우기용
	std::vector<Network_Message> Launcher_Msg_Vec;
	std::vector<Network_Message> Lobby_Msg_Vec;
	// Send/Recv 용 구조체 선언
	// Lobby
	flatbuffers::FlatBufferBuilder Lobby_Builder;
	S2C_Packet* Lobby_Recv_Packet = new S2C_Packet();
	C2S_Packet Lobby_Send_Packet;
	// Launcher
	flatbuffers::FlatBufferBuilder Launcher_Builder;
	C2S_Packet* Launcher_Recv_Packet = new C2S_Packet();
	S2C_Packet Launcher_Send_Packet;
	// KeepAlive
	flatbuffers::FlatBufferBuilder KeepAlive_Builder;
	C2S_Packet* KeepAlive_Recv_Packet = new C2S_Packet();
	S2C_Packet KeepAlive_Send_Packet;

	/// 서버 정보관리
	// 연결된 클라이언트을 파악하기 위함. (소켓에 해당하는 로그인된 아이디값을 저장해둔다)
	std::map<std::string, User_Data*> Logged_In_User_Data;


private:
	void LobbySideLogic();
	void KeepAlive();
	void Login_Certify(SOCKET _Socket_Num, C2S_Packet* _C2S_Msg);

public:
	LoginManager();
	~LoginManager();

	void LauncherSideLogic();

};

