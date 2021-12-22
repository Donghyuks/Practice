
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

						printf_s("[Recv_Player_Position] x : %.2f, y : %.2f, z : %2.f", Player_Data->pos()->x(), Player_Data->pos()->y(), Player_Data->pos()->z());
					}


					//const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					//
					//const Eater::PlayerData::Player* Recv_Player_Data = Eater::PlayerData::GetPlayer(Recv_Data_Ptr);

					//auto Hp = Recv_Player_Data->hp();
					//auto Postion_Vector = Recv_Player_Data->pos();
					//auto Mov_Vec = Recv_Player_Data->mov_vector();

					//std::cout << "HP : " << Hp << " Position : " << Postion_Vector->x() << ", " << Postion_Vector->y() << ", " << Postion_Vector->z();
					//std::cout << " Mov_Vector : " << Mov_Vec->x() << ", " << Mov_Vec->y() << ", " << Mov_Vec->z() << std::endl;
				}

				delete S2C_Msg.Packet;
				S2C_Msg.Packet = nullptr;
			}

			Msg_Vec.clear();
		}
	}
}