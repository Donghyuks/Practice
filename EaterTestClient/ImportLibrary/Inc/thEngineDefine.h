#pragma once


/// ��Ÿ ���ǵ�.

/// wrold ��ǥ�� �� ũ��. DIP ���� ���.
// ������ �� ȭ�� ũ��.
#define WORLD_SCREEN_X 1920
#define WORLD_SCREEN_Y 1080

// Text�� ��µ� �ִ� ����.
#define MAX_WORD 512

// ���ĵ�
#define _PI		3.141592f
#define _1RAD	0.017453f
#define ToRadian( degree ) ( _1RAD * degree )
#define ToDegree( radian ) ( radian * 180 / _PI )

// �浹üũ Ÿ�Ե�.
enum class CollisionType
{
	None,
	CircleCheck,
	AABB,
	OBB,
};

// ������ ����.
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

