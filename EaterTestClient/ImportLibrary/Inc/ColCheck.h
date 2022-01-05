#pragma once

#include "thEngineHeader.h"

class Transform;

class ColCheck : public Component
{
private:
	// �⺻ ��ǥ�� ���� ����.
	D2DEngine* pthEngine;
	CollisionType Col_Type;

	// ���� �Ϳ� ���� ����.
	Transform* Object_transform;
	D2D1_POINT_2F Col_Size;

	// ���� Ÿ�ٿ� ���� ������
	Transform* Target_transform;
	D2D1_POINT_2F Target_Col_Size;

	// OBB �� ���� ���ͷ��� ���翵 �� max, min ���� ���� ���� ������.
	float rc1_max, rc1_min;
	float rc2_max, rc2_min;

	// OBB�� üũ�ϱ� ���� �簢�� �ΰ��� ���� ���غ��� 2����.
	Vector2<float> Standard_Vec[4];

	// Collsion ������ �����ϱ� ���� ����.
	bool Is_Col = false;
public:
	ColCheck(D2DEngine* _thEngine, D2D1_POINT_2F _Col_Size, Transform* _Obj_Transform);
	virtual ~ColCheck();

	virtual void Draw();

	virtual void Update();

	virtual void Release();
	
	void SetTarget(Transform* _Target, D2D1_POINT_2F _Target_Col_Size, CollisionType _Col_Type = CollisionType::AABB);

	/// Collision Size�� �޾ƿ´�.
	D2D1_POINT_2F GetColSize();
	Transform* GetTransform();
	/// Collision ���θ� �޾ƿ´�.
	bool GetIsCollision();

	/// Collision Check �ϴ� �κ�.
	void CollisionCheck();

	/// Collision üũ�ϴ� ������
	


	bool CircleColCheck(float _Circle1_X, float _Circle1_Y, float _Circle1_Radius, float _Circle2_X, float _Circle2_Y, float _Circle2_Radius);
	bool AABBColCheck(float _Left1, float _Top1, float _Right1, float _Bottom1,
		float _Left2, float _Top2, float _Right2, float _Bottom2);
	bool OBBColCheck(Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);
	void OBBSetMinMax(int Check_Vector_Num, Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);
	bool OBBIsRange(Rotate_Point_Rect rc1, Rotate_Point_Rect rc2);

	/// �����ص� Ÿ���� �����.
	void ReleaseTarget();
};

