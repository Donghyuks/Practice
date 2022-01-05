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

using namespace std;

// 싱글톤으로 생성한 D2D 엔진 쉽게 쓰려구
#define MYD2D D2DEngine::GetInctance()

class Object;
class DHTimer;
class DHKeyIO;
class DHNetWorkAPI;
struct C2S_Packet;

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

	unsigned short					m_Sequence = 0;
	unsigned short					m_Animation = ANIMATION_IDLE;
	unsigned short					m_Character = 0;
	double							m_Speed = 0.f;
	DirectX::SimpleMath::Vector3	m_Position;
};


/// 게임 진행 총괄 매니저.
class GameManager
{
private:
	HWND g_hwnd;
	MSG msg;

	DHNetWorkAPI* m_Network = nullptr;
	C2S_Packet* _Send_Packet = nullptr;

	DHKeyIO* m_KeyIO = nullptr;
	DHTimer* m_Timer = nullptr;

	vector<Object*> Obj_List;

	// 시작할때 서버에서 알려줌.
	unsigned short m_Player_Number = 0;

	std::map<unsigned short, Player*> m_Player;

public:
	// 게임용 윈도 프레임웍을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 메인 게임 루프. 실제 게임이 진행되는 반복구문
	void Loop();
	void GameLoop();

	// 게임을 정리한다.
	void Finalize();

	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

