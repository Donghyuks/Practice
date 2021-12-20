#pragma once

/*
	2021/12/20 10:38 - CDH
	
	< 변경사항 >
		1. 추후 로비매니저를 통해서 로비서버와의 통신을 진행할 예정.. ( 로비서버-서버, 게임서버-클라이언트 관계가 됨)
		
*/

#include "SharedDataStruct.h"
#include <string>

class DHNetWorkAPI;

class LobbyManager
{
// SingleTon
private:
	LobbyManager();

	static LobbyManager* m_Instance;

public:
	~LobbyManager();

	void Initialize(std::string _IP, int PORT);
	static LobbyManager* GetInstance();
};

