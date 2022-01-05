#pragma once

#include "thEngineHeader.h"

class Transform;

class ColCheck : public Component
{
private:
	// 기본 좌표들 설정 값들.
	D2DEngine* pthEngine;
	CollisionType Col_Type;

	// 현재 것에 대한 정보.
	Transform* Object_transform;
	D2D1_POINT_2F Col_Size;

	// 비교할 타겟에 대한 정보들
	Transform* Target_transform;
	D2D1_POINT_2F Target_Col_Size;

	// OBB 중 기준 벡터로의 정사영 후 max, min 길이 값을 각각 가져옴.
	float rc1_max, rc1_min;
	float rc2_max, rc2_min;

	// OBB를 체크하기 위한 사각형 두개의 각각 기준벡터 2개씩.
	Vector2<float> Standard_Vec[4];

	// Collsion 정보를 저장하기 위한 변수.
	bool Is_Col = false;
public:
	ColCheck(D2DEngine* _thEngine, D2D1_POINT_2F _Col_Size, Transform* _Obj_Transform);
	virtual ~ColCheck();

	virtual void Draw();

	virtual void Update();

	virtual void Release();
	
	void SetTarget(Transform* _Target, D2D1_POINT_2F _Target_Col_Size, CollisionType _Col_Type = CollisionType::AABB);

	/// Collision Size를 받아온다.
	D2D1_POINT_2F GetColSize();
	Transform* GetTransform();
	/// Collision 여부를 받아온다.
	bool GetIsCollision();

	/// Collision Check 하는 부분.
	void CollisionCheck();

	/// Collision 체크하는 로직들
	


	bool CircleColCheck(float _Circle1_X, float _Circle1_Y, float _Circle1_Radius, float _Circle2_X, float _Circle2_Y, float _Circle2_Radius);
	bool AABBColCheck(float _Left1, float _Top1, float _Right1, float _Bottom1,
		float _Left2, float _Top2, float _Right2, float _Bottom2);
	bool OBBColCheck(Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);
	void OBBSetMinMax(int Check_Vector_Num, Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);
	bool OBBIsRange(Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);

	/// 설정해둔 타겟을 지운다.
	void ReleaseTarget();
};

