
#define CONNECT_IP "127.0.0.1"
#define CONNET_PORT 729

#include <iostream>
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"

int main()
{
	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;

	DHNetWorkAPI* my_NetWork = new DHNetWorkAPI();
	my_NetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	// 연결 대기
	while (!my_NetWork->Connect(CONNET_PORT, CONNECT_IP));

	while (true)
	{
		if (my_NetWork->Recv(Msg_Vec))
		{
			for (auto& S2C_Msg : Msg_Vec)
			{
				Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

				if (Recv_Packet->Packet_Type == S2C_PLAYER_MOVE_RES)
				{
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
					
					const Eater::PlayerData::Player* Recv_Player_Data = Eater::PlayerData::GetPlayer(Recv_Data_Ptr);

					auto Hp = Recv_Player_Data->hp();
					auto Postion_Vector = Recv_Player_Data->pos();
					auto Mov_Vec = Recv_Player_Data->mov_vector();

					std::cout << "HP : " << Hp << " Position : " << Postion_Vector->x() << ", " << Postion_Vector->y() << ", " << Postion_Vector->z();
					std::cout << " Mov_Vector : " << Mov_Vec->x() << ", " << Mov_Vec->y() << ", " << Mov_Vec->z() << std::endl;
				}

				delete S2C_Msg.Packet;
				S2C_Msg.Packet = nullptr;
			}

			Msg_Vec.clear();
		}
	}
}