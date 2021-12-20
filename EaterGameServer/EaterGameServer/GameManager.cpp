#include "GameManager.h"
#include "DHNetWorkAPI.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"
#include "DHTimer.h"

GameManager::GameManager()
{
	// 타이머 생성
	m_Timer = new DHTimer;
	m_Timer->SetFrame(60);

	// 로비 서버와 통신(클라이언트 입장)
	//m_Lobby_Client = new DHNetWorkAPI();
	//m_Lobby_Client->Initialize(DHNetWork_Name::DHNet);
	//m_Lobby_Client->Connect(729, "127.0.0.1");

	// 게임클라이언트를 받기 위한 게임서버 생성
	m_Game_Server = new DHNetWorkAPI();
	m_Game_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_SIMPLE);
	m_Game_Server->Accept(729, 4, 4);

	g_Packet_Send_Thread = new std::thread(std::bind(&GameManager::S2C_Loop, this));
}

GameManager::~GameManager()
{

}

void GameManager::S2C_Loop()
{
	// 게임서버는 계속 재사용되므로 이 로직은 계속 실행되어야 한다.
	while (true)
	{
		while (!Is_Game_End)
		{
			// 60 프레임
			if (m_Timer->Ready_Frame())
			{
				// 움직임전송.
				S2C_Player_Move();
			}

			Sleep(0);
		}

		// 현재 게임서버에 연결되어 플레이중인 클라이언트가 존재하지 않는다면..
		Sleep(0);
	}
}

void GameManager::GameLogic()
{
	while (!Is_Game_End)
	{
		// 주기적으로 받은 패킷이 있으면 확인한다.
		if (m_Game_Server->Recv(Msg_Vec))
		{
			// 클라이언트로부터 받은 데이터들로 로직을 처리..
			for (auto Msg_Packet : Msg_Vec)
			{
				// 클라이언트의 소켓 번호
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// 클라이언트가 보낸 메세지
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// 플레이어 이동에 대한 처리..
				if (C2S_Msg->Packet_Type == C2S_PLAYER_MOVE_REQ)
				{
					if (C2S_Player_Move() == false)
					{
						// 로직실패..
					}
				}

				// 서버가 살아있는지 검사하는 패킷.
				if (C2S_Msg->Packet_Type == C2S_KEEP_ALIVE_CHECK_REQ)
				{
					if (C2S_Keep_Alive() == false)
					{
						// 로직실패..
					}
				}

				// 사용한데이터 해제.
				delete Msg_Packet.Packet;
				C2S_Msg = nullptr;
			}
		}

		Sleep(0);
	}
}

void GameManager::S2C_Player_Move()
{
	// FlatBuffer를 사용하기 위한 빌더
	flatbuffers::FlatBufferBuilder _Builder;
	// 클라이언트로 보내는 메세지.
	_S2C_Msg = new S2C_Packet;

	// 플레이어 데이터 전송 예시..
	auto Name = _Builder.CreateString("Player01");
	auto Position = Eater::PlayerData::Vec3(0.1f, 0.f, 0.f);
	auto Mov_Vec = Eater::PlayerData::Vec3(0.5f, 0.f, 0.f);
	UINT32 HP = 100;
	UINT32 Mana = 50;

	// 데이터 직렬화 완료.
	_Builder.Finish(CreatePlayer(_Builder, Name, &Position, &Mov_Vec, HP, Mana));

	_S2C_Msg->Packet_Type = S2C_PLAYER_MOVE_RES;
	_S2C_Msg->Packet_Size = _Builder.GetSize();
	memcpy_s(_S2C_Msg->Packet_Buffer, _Builder.GetSize(), _Builder.GetBufferPointer(), _Builder.GetSize());

	// BroadCast
	m_Game_Server->Send(_S2C_Msg);

	_Builder.Clear();
}

BOOL GameManager::C2S_Player_Move()
{

	return true;
}

BOOL GameManager::C2S_Keep_Alive()
{

	return true;
}
