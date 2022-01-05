#pragma once


/// 기타 정의들.

/// wrold 좌표계 상 크기. DIP 개념 사용.
// 기준이 될 화면 크기.
#define WORLD_SCREEN_X 1920
#define WORLD_SCREEN_Y 1080

// Text상 출력될 최대 숫자.
#define MAX_WORD 512

// 공식들
#define _PI		3.141592f
#define _1RAD	0.017453f
#define ToRadian( degree ) ( _1RAD * degree )
#define ToDegree( radian ) ( radian * 180 / _PI )

// 충돌체크 타입들.
enum class CollisionType
{
	None,
	CircleCheck,
	AABB,
	OBB,
};

// 포인터 해제.
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

