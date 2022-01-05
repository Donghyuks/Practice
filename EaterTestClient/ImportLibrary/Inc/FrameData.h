#pragma once

#include "thEngineHeader.h"

class Frame_Data
{
public:
	// 한프레임에 해당하는 비트맵 내의 위치.
	float Draw_PosX;
	float Draw_PosY;

	// 해당 프레임에 할당된 속도 (상대 값)
	float Frame_Speed;

	/// 추가로 들어갈 기타 속성이있으면 여기에 정의..
	

public:
	Frame_Data(float _Draw_PosX, float _Draw_PosY, float _Frame_Speed = 1);
	~Frame_Data();

};
