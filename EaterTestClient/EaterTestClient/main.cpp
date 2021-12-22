
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

	// Recv ������ �޾ƿ���
	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;

	DHNetWorkAPI* my_NetWork = new DHNetWorkAPI();
	my_NetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	// ���� ���
	while (!my_NetWork->Connect(CONNET_PORT, CONNECT_IP));

	C2S_Packet* Send_Packet = nullptr;
	Send_Packet = new C2S_Packet();
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE;
	Send_Packet->Packet_Size = 0;

	// �ε��� �������� �˸�.
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
					// ���� ���� �޼����� �޾��� ��.
					if (Recv_Packet->Packet_Type == S2C_START_GAME)
					{
						// ���ӽ��� flag On
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
					// Vector�� ������
					auto Player_List_Vector = Recv_Player_List_Data->players();
					
					// Vector�� ��ȸ�ϸ� �����͸� ��ȸ�Ѵ�.
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

					//// �޼��� ������ �׽�Ʈ.
					//flatbuffers::FlatBufferBuilder m_Builder;
					//
					//auto _Pos = Eater::PlayerData::Vec3(1.f, 1.f, 1.f);
					//auto _Mov_Vec = Eater::PlayerData::Vec3(-1.f, -1.f, -1.f);

					//auto _Player_Data = Eater::PlayerData::CreatePlayer(m_Builder, EATER_PLAYER_01, &_Pos, &_Mov_Vec);

					//m_Builder.Finish(_Player_Data);

					//// ��Ŷ ����� �ٿ� �����ش�.
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