
#define CONNECT_IP "127.0.0.1"
#define CONNET_PORT 729

#include <iostream>
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"

int main()
{
	bool Is_Game_Start = false;

	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;

	DHNetWorkAPI* my_NetWork = new DHNetWorkAPI();
	my_NetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	// 연결 대기
	while (!my_NetWork->Connect(CONNET_PORT, CONNECT_IP));

	C2S_Packet* Send_Packet = nullptr;
	Send_Packet = new C2S_Packet();
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE;
	Send_Packet->Packet_Size = 0;

	// 로딩이 끝났음을 알림.
	my_NetWork->Send(Send_Packet);

	while (true)
	{
		if (my_NetWork->Recv(Msg_Vec))
		{
			for (auto& S2C_Msg : Msg_Vec)
			{
				Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

				if (!Is_Game_Start)
				{
					// 게임 시작 메세지를 받았을 때.
					if (Recv_Packet->Packet_Type == S2C_START_GAME)
					{
						// 게임시작 flag On
						Is_Game_Start = true;

						delete S2C_Msg.Packet;
						S2C_Msg.Packet = nullptr;
						continue;
					}
				}

				if (Recv_Packet->Packet_Type == S2C_PLAYER_MOVE_RES)
				{
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;

					// Version 2
					const Eater::PlayerData::PlayerList* Recv_Player_List_Data = Eater::PlayerData::GetPlayerList(Recv_Data_Ptr);
					// Vector의 포인터
					auto Player_List_Vector = Recv_Player_List_Data->players();
					
					// Vector를 순회하며 데이터를 조회한다.
					for (int i = 0; i < Player_List_Vector->size(); i++)
					{
						auto Player_Data = Player_List_Vector->Get(i);

						if (EATER_PLAYER_01 == Player_Data->identifier())
						{
							printf_s("[PLAYER 1 Position] x : %.2f, y : %.2f, z : %2.f\n", Player_Data->pos()->x(), Player_Data->pos()->y(), Player_Data->pos()->z());
						}
						else if (EATER_PLAYER_02 == Player_Data->identifier())
						{
							printf_s("[PLAYER 2 Position] x : %.2f, y : %.2f, z : %2.f\n", Player_Data->pos()->x(), Player_Data->pos()->y(), Player_Data->pos()->z());
						}
					}

					//// 메세지 보내기 테스트.
					//flatbuffers::FlatBufferBuilder m_Builder;
					//
					//auto _Pos = Eater::PlayerData::Vec3(1.f, 1.f, 1.f);
					//auto _Mov_Vec = Eater::PlayerData::Vec3(-1.f, -1.f, -1.f);

					//auto _Player_Data = Eater::PlayerData::CreatePlayer(m_Builder, EATER_PLAYER_01, &_Pos, &_Mov_Vec);

					//m_Builder.Finish(_Player_Data);

					//// 패킷 헤더를 붙여 보내준다.
					//Send_Packet->Packet_Type = C2S_PLAYER_MOVE_REQ;
					//Send_Packet->Packet_Size = m_Builder.GetSize();
					//memcpy_s(Send_Packet->Packet_Buffer, m_Builder.GetSize(), m_Builder.GetBufferPointer(), m_Builder.GetSize());

					//my_NetWork->Send(Send_Packet);
				}

				delete S2C_Msg.Packet;
				S2C_Msg.Packet = nullptr;
			}

			Msg_Vec.clear();
		}
	}
}