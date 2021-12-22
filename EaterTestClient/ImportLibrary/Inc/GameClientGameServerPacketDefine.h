#pragma once

/*
	2021/12/20 10:54 - CDH
	
	< 변경사항 >
		1. GameClient(Client), GameServer(Server) 간의 패킷 정의.
		
*/

// Player Identifier
#define EATER_PLAYER_01					20000
#define EATER_PLAYER_02					20001
#define EATER_PLAYER_03					20002
#define EATER_PLAYER_04					20003

// Client To Server
#define C2S_LOADING_COMPLETE			1
#define C2S_PLAYER_MOVE_REQ				1001
#define C2S_KEEP_ALIVE_CHECK_REQ		1002

// Server To Client
// Loop
#define S2C_PLAYER_MOVE_RES				1501
#define S2C_KEEP_ALIVE_CHECK_RES		1502
#define S2C_MONSTER_UPDATE				1503
#define S2C_BOSS_UPDATE					1504
// Non-Loop
#define S2C_START_GAME					501