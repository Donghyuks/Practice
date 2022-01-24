#pragma once

#include "D2DEngine.h"
#include "GameClientGameServerPacketDefine.h"
#include "transform.h"
#include "Rigidbody2D.h"
#include "Physics.h"
#include "ColCheck.h"
#include "Animation.h"
#include "Object.h"
#include "SimpleMath.h"
#include <map>
#include <queue>
#include <chrono>

// 디버그용 콘솔창
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;

// 싱글톤으로 생성한 D2D 엔진 쉽게 쓰려구
#define MYD2D D2DEngine::GetInctance()

// 최대 시퀀스 개수 (unsigned int)
#define MAX_SEQUENCE		0xFFFFFFFF

class Object;
class DHTimer;
class DHKeyIO;
class DHNetWorkAPI;
struct C2S_Packet;
class PhysData;

struct Enemy
{
	unsigned short	Index;
	DirectX::SimpleMath::Vector3	Position;
};

struct ManaStone
{
	unsigned short	Index;
	DirectX::SimpleMath::Vector3	Position;
};

struct Player
{
	Player() {};
	Player(Player& _rhs)
	{
		m_Sequence = _rhs.m_Sequence;
		m_Animation = _rhs.m_Animation;
		m_Character = _rhs.m_Character;
		m_Speed = _rhs.m_Speed;
		m_Position = _rhs.m_Position;
	}

	PhysData*						m_PhysX = nullptr;
	unsigned int					m_Sequence = 0;
	unsigned short					m_Animation = ANIMATION_IDLE;
	unsigned short					m_Character = 0;
	double							m_Speed = 0.f;
	DirectX::SimpleMath::Vector3	m_Position;
	DirectX::SimpleMath::Vector3	m_Prev_Position;			// 다른 클라이언트들의 보간을 위한 이전 포지션값.
};

struct FrameData
{
	DirectX::SimpleMath::Vector3 m_Predict_Position;
	DirectX::SimpleMath::Vector3 m_Mov_Vec3;
	unsigned int		m_Sequence	= 0;			// 시퀀스 번호.
	// KeyIO 정보
	bool				m_forward	= false;
	bool				m_back		= false;
	bool				m_right		= false;
	bool				m_left		= false;
	bool				m_dash		= false;
	bool				m_skill1	= false;
	bool				m_skill2	= false;
};

/// 게임 진행 총괄 매니저.
class GameManager
{
private:
	HWND g_hwnd;
	MSG msg;

	std::chrono::time_point<std::chrono::system_clock> Prev_Time;
	std::chrono::time_point<std::chrono::system_clock> Current_Time;
	std::chrono::duration<double> Passed_Time = std::chrono::duration<double>::zero();

	DHNetWorkAPI* m_Network = nullptr;
	C2S_Packet* _Send_Packet = nullptr;

	DHKeyIO* m_KeyIO = nullptr;
	DHTimer* m_Timer = nullptr;
	DHTimer* m_KeyIO_Timer = nullptr;

	std::vector<Enemy> Enemy_List;
	std::vector<ManaStone> Mana_List;

	/// 추후 게임런처에서 자신의 Key 값을 알려줄 예정 현재는 임시로 Input을 받음
	unsigned int m_Key = 0;
	// 시작할때 서버에서 내가 무슨 플레이언지 알려줌
	unsigned short m_Player_Number = 0;
	// Dtime 기록.
	double m_Dtime = 0.f;
	// 클라이언트에서 선실행하는 데이터
	std::queue<FrameData*> m_Client_Predict;
	// BackUp 용 데이터
	std::queue<FrameData*> m_Client_BackUp;
	// 클라이언트의 움직임을 기억할 방향벡터 (한프레임 처리후 서버에 보냄)
	DirectX::SimpleMath::Vector3 m_Client_Move_Vector;
	// 게임을 진행하는 상대 플레이어
	std::map<unsigned short, Player*> m_Player_List;
	Player* m_Current_Player	= nullptr;		// 현재 플레이어 정보
	Player* m_Dest_Player		= nullptr;		// 목적 플레이어 정보 ( 목적으로 하는 플레이어 )
	FrameData* _FData = nullptr;		// 한프레임 데이터 기록
	// 네트워크로부터 역할을 부여받았는가? (게임이 시작되었는가?)
	bool m_Is_Start = false;
	// 테스트..
	double m_RTT = 0.f;
	// 현재 클라이언트가 서버로부터 데이터를 받고 지난 시간
	double m_Passed_Client_Time = 0.f;
	// 데이터 정보 오차범위
	double m_Epsilon = 0.01f;

public:
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();
	void GameLoop();
	void KeyIO_Recording();
	void PhysX_World_Update();

	// 게임을 정리한다.
	void Finalize();

	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

