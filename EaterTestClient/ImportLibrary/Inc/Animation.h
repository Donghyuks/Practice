#pragma once

#include <vector>
#include <string>
#include "Component.h"

class D2DEngine;
class Frame_Data;
class Transform;

class Animation : public Component
{
private:
	// 엔진 포인터.
	D2DEngine* pthEngine;
	// 트랜스폼 포인터.
	Transform* Object_transform;

	// 한장짜리 애니메이션 데이터.
	ID2D1Bitmap* Animation_Bitmap;

	// 애니메이션 재생용 데이터.
	vector<Frame_Data*> F_Data;
	
	// 한 컷 사이즈
	float One_Cut_SizeX;
	float One_Cut_SizeY;

	// 애니메이션 총 개수
	int Image_Cut_Count;
	// 애니메이션 재생 시간 ( 속도의 역수 ).
	float Ani_Time;

	// 한 프레임이 실행될때마다 1씩 증가하는 경과 프레임 저장.
	int Pass_Frame = 0;
	// 한 컷당 재생될 시간.
	float One_Cut_Time = 0;
	// 현재 재생 애니메이션 프레임.
	int Current_Frame = 0;

public:
	Animation(Transform* _Obj_Transform, D2DEngine* _pthEngine, int _Image_Cut_Count, float _Ani_Time, string _Image_File_Path, string _Image_Data = "");
	~Animation();

	virtual void Frame_Reset();

	virtual void Update();
	virtual void Draw();
	virtual void Release();
};

