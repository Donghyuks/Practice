
#include <iostream>
#include "DHNetWorkAPI.h"
#include "PortIPDefine.h"
#include "SharedDataStruct.h"
#include "LauncherLoginPacketDefine.h"
#include "LoginLauncher_generated.h"

int main()
{
	bool Is_Game_Start = false;

	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;
	S2C_Packet* Recv_Packet = nullptr;

	DHNetWorkAPI* my_NetWork = new DHNetWorkAPI();
	my_NetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	printf_s("[LauncherTest] 아이디와 비밀번호를 입력해주세요.\n");

	std::string _Input_ID;
	std::string _Input_Password;

	std::cin >> _Input_ID >> _Input_Password;

	// 연결 대기
	while (!my_NetWork->Connect(LOGIN_SERVER_PORT, CONNECT_IP));

	C2S_Packet* Send_Packet = new C2S_Packet;

	// 메세지 보내기 테스트.
	flatbuffers::FlatBufferBuilder m_Builder;
	
	auto _ID = m_Builder.CreateString(_Input_ID);
	auto _Password = m_Builder.CreateString(_Input_Password);

	auto _Login_Data = Eater::LoginLauncher::CreateLoginReqData(m_Builder, _ID, _Password);

	m_Builder.Finish(_Login_Data);

	// 패킷 헤더를 붙여 보내준다.
	Send_Packet->Packet_Type = C2S_LOGIN_SAFE_REQ;
	Send_Packet->Packet_Size = m_Builder.GetSize();
	memcpy_s(Send_Packet->Packet_Buffer, m_Builder.GetSize(), m_Builder.GetBufferPointer(), m_Builder.GetSize());

	// 로그인 시도.
	my_NetWork->Send(Send_Packet);

	while (true)
	{
		if (my_NetWork->Recv(Msg_Vec))
		{
			for (auto& S2C_Msg : Msg_Vec)
			{
				Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);

				if (Recv_Packet->Packet_Type == S2C_LOGIN_SAFE_RES)
				{
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;

					const auto Recv_Login_Result = flatbuffers::GetRoot<Eater::LoginLauncher::LoginResData>(Recv_Data_Ptr);
					
					auto _Login_Result = Recv_Login_Result->result();
					auto _Login_Identifier = Recv_Login_Result->key();

				}
				else if (Recv_Packet->Packet_Type == S2C_KEEP_ALIVE_CHECK_REQ)
				{
					const uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;

					const auto Recv_Login_Result = flatbuffers::GetRoot<Eater::LoginLauncher::RealTimeData>(Recv_Data_Ptr);

					auto Recv_Friend_State_Vector = Recv_Login_Result->friendstate();

					for (int i = 0; i < Recv_Friend_State_Vector->size(); i++)
					{
						auto Friend_Data = Recv_Friend_State_Vector->Get(i);

						auto _User_ID = Friend_Data->id()->str();

						printf_s("등록된 친구%d : %s \n",i, _User_ID.c_str());
					}

				}

				delete S2C_Msg.Packet;
				S2C_Msg.Packet = nullptr;
			}

			Msg_Vec.clear();
		}
	}
}