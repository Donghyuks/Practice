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

// ����׿� �ܼ�â
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

using namespace std;

// �̱������� ������ D2D ���� ���� ������
#define MYD2D D2DEngine::GetInctance()

// �ִ� ������ ���� (unsigned int)
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
	DirectX::SimpleMath::Vector3	m_Prev_Position;			// �ٸ� Ŭ���̾�Ʈ���� ������ ���� ���� �����ǰ�.
};

struct FrameData
{
	DirectX::SimpleMath::Vector3 m_Predict_Position;
	DirectX::SimpleMath::Vector3 m_Mov_Vec3;
	unsigned int		m_Sequence	= 0;			// ������ ��ȣ.
	// KeyIO ����
	bool				m_forward	= false;
	bool				m_back		= false;
	bool				m_right		= false;
	bool				m_left		= false;
	bool				m_dash		= false;
	bool				m_skill1	= false;
	bool				m_skill2	= false;
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
	DHTimer* m_KeyIO_Timer = nullptr;

	std::vector<Enemy> Enemy_List;
	std::vector<ManaStone> Mana_List;

	/// ���� ���ӷ�ó���� �ڽ��� Key ���� �˷��� ���� ����� �ӽ÷� Input�� ����
	unsigned int m_Key = 0;
	// �����Ҷ� �������� ���� ���� �÷��̾��� �˷���
	unsigned short m_Player_Number = 0;
	// Dtime ���.
	double m_Dtime = 0.f;
	// Ŭ���̾�Ʈ���� �������ϴ� ������
	std::queue<FrameData*> m_Client_Predict;
	// BackUp �� ������
	std::queue<FrameData*> m_Client_BackUp;
	// Ŭ���̾�Ʈ�� �������� ����� ���⺤�� (�������� ó���� ������ ����)
	DirectX::SimpleMath::Vector3 m_Client_Move_Vector;
	// ������ �����ϴ� ��� �÷��̾�
	std::map<unsigned short, Player*> m_Player_List;
	Player* m_Current_Player	= nullptr;		// ���� �÷��̾� ����
	Player* m_Dest_Player		= nullptr;		// ���� �÷��̾� ���� ( �������� �ϴ� �÷��̾� )
	FrameData* _FData = nullptr;		// �������� ������ ���
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
	void KeyIO_Recording();
	void PhysX_World_Update();

	// ������ �����Ѵ�.
	void Finalize();

	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

