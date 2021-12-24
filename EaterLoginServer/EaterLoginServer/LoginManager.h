#pragma once

/*
	2021/12/23 15:58 - CDH
	
	< �������� >
		1. ������ ������ ������ ������ ���� ���� ���̺�(���̵� , ���Ϲ�ȣ) �� �����Ѵ�.
		2. ���� ������ ���ؼ� ģ�� ����� DB���� ��ȸ�Ͽ� ���� ����Ѵ�.
		3. �ش� ������ �����ٸ� ���Ϲ�ȣ�� invalid �������� �ٲ۴�.
		4. �����ð����� �ش� ���̺� ������ �����Ѵ�. (���� ����)
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
	// Socket��ȣ�� INVAILD �� ���� Offline �̶�� ���̴�.
	std::atomic<unsigned int> Socket_Num = INVALID_SOCKET;
	std::atomic<unsigned int> User_State = USER_OFFLINE;
	std::vector<std::string> Friend_List;
	std::vector<std::string> Friend_Request_List;	// ���ı���..
};

class LoginManager
{
private:
	CRITICAL_SECTION g_CS;

private:
	/// ��Ʈ��ũ ���� �� ���� ����
	// ���� ��ó�� ������ ��Ʈ��ũ (������ ����)
	DHNetWorkAPI* m_Connect_Launcher = nullptr;
	// �κ� ������ ������ ��Ʈ��ũ (Ŭ���̾�Ʈ�� ����)
	DHNetWorkAPI* m_Connect_Lobby = nullptr;
	// �κ� �ʰ��� ������ ������ ���� ������
	std::thread* m_Lobby_Thread = nullptr;
	// ���� �ֱ�� ������ ó���ϱ� ���� ������
	std::thread* m_Keep_Alive_Thread = nullptr;

	/// DB ����
	DHDB* m_DB = nullptr;

	/// Timer
	DHTimer* m_Timer = nullptr;

	/// ������ �ʿ��� ����
	// �κ�� ����Ǿ�����? (�κ�� ������ �ȵǾ��ִٸ� �α��μ����� �����ϸ� �ȵȴ�.)
	std::atomic<bool> Is_Connect_Lobby = false;
	// Recv ���� �����
	std::vector<Network_Message> Launcher_Msg_Vec;
	std::vector<Network_Message> Lobby_Msg_Vec;
	// Send/Recv �� ����ü ����
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

	/// ���� ��������
	// ����� Ŭ���̾�Ʈ�� �ľ��ϱ� ����. (���Ͽ� �ش��ϴ� �α��ε� ���̵��� �����صд�)
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

