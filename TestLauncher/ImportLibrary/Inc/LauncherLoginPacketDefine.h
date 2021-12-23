#pragma once

/*
	2021/12/23 10:29 - CDH
	
	< 변경사항 >
		1.  GameLauncher(Client), LoginServer(Server) 간의 패킷 정의.
		
*/

// 결과값 처리를 위한 부분
#define LOGIN_ID_FAIL				0b0000
#define LOGIN_PASSWORD_FAIL			0b0010
#define LOGIN_SUCCESS				0b0100

// Client To Server
#define	C2S_KEEP_ALIVE_CHECK_RES	5001
#define	C2S_LOGIN_SAFE_REQ			5002
#define	C2S_ADD_FRIEND_REQ			5003
#define	C2S_ADD_FRIEND_CONFIRM		5004
#define	C2S_LOGIN_SERVER_START		5005

// Server To Client
#define	S2C_KEEP_ALIVE_CHECK_REQ	5501
#define	S2C_LOGIN_SAFE_RES			5502
#define	S2C_ADD_FRIEND_RES			5503

