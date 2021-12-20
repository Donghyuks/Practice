#pragma once

/*
	2021/12/20 10:54 - CDH
	
	< 변경사항 >
		1. GameClient(Client), GameServer(Server) 간의 패킷 정의.
		
*/

// Client To Server
#define C2S_PLAYER_MOVE_REQ				1000
#define C2S_KEEP_ALIVE_CHECK_REQ		1001

// Server To Client
// Loop
#define S2C_PLAYER_MOVE_RES				1500
#define S2C_KEEP_ALIVE_CHECK_RES		1501
#define S2C_MONSTER_UPDATE				1502
#define S2C_BOSS_UPDATE					1503
// Non-Loop