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

using namespace std;

// �̱������� ������ D2D ���� ���� ������
#define MYD2D D2DEngine::GetInctance()

// �ִ� ������ ���� (unsigned short ���̹Ƿ� ���� 6����..)
#define MAX_SEQUENCE		60000

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
	DirectX::SimpleMath::Vector3	m_Prev_Position;			// �ٸ� Ŭ���̾�Ʈ���� ������ ���� ���� �����ǰ�.
};

struct FrameData
{
	unsigned short					m_Sequence;			// ������ ��ȣ.
	DirectX::SimpleMath::Vector3	m_Position;			// ���� �����ӿ����� ��ġ (���⺤�� ������� ��)
	DirectX::SimpleMath::Vector3	m_Mov_Vector;		// �̵��� ����
	double							m_dtime;			// dtime ��
};

/// ���� ���� �Ѱ� �Ŵ���.
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

	vector<Object*> Obj_List;

	// �����Ҷ� �������� ���� ���� �÷��̾��� �˷���
	unsigned short m_Player_Number = 0;
	// Dtime ���.
	double m_Dtime = 0.f;
	// Ŭ���̾�Ʈ���� �������ϴ� ������
	std::queue<FrameData*> m_Client_Predict;
	// Ŭ���̾�Ʈ�� �������� ����� ���⺤�� (�������� ó���� ������ ����)
	DirectX::SimpleMath::Vector3 m_Client_Move_Vector;
	// ������ �����ϴ� �÷��̾��
	std::map<unsigned short, Player*> m_Player_List;
	Player* m_Player = nullptr;		// ���� ���� ���� �÷��̾�����
	// ��Ʈ��ũ�κ��� ������ �ο��޾Ҵ°�? (������ ���۵Ǿ��°�?)
	bool m_Is_Start = false;
	// �׽�Ʈ..
	double m_RTT = 0.f;
	// ���� Ŭ���̾�Ʈ�� �����κ��� �����͸� �ް� ���� �ð�
	double m_Passed_Client_Time = 0.f;
	// ������ ���� ��������
	double m_Epsilon = 0.01f;

public:
	// ���ӿ� ���� �����ӿ��� �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);

	// ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
	void Loop();
	void GameLoop();
	void TestLoop();

	// ������ �����Ѵ�.
	void Finalize();

	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

