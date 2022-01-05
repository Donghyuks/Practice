#pragma once

class D2DEngine;
class Transform;
class Rigidbody2D;

class Physics : public Component
{
private:
	D2DEngine* pthEngine;

	// 현재 오브젝트에 대한 transform
	Transform* Object_transform;
	// 타겟 오브젝트에 대한 transform
	Transform* Target_transform;

	// 현재 오브젝트에 대한 RigidBody
	Rigidbody2D* Object_Rigidbody2D;
	// 타겟 오브젝트에 대한 RigidBody
	Rigidbody2D* Target_Rigidbody2D;

	// 콜리젼 타입에 대해 물리연산이 달라질 수도 있다.
	CollisionType Col_Type;
	// 충돌 마찰계수. (손실량)
	float Crash_Friction;
	// 탄성 계수
	float Shout_Friction;

public:
	Physics(D2DEngine* _pthEngine, Transform* _pObject_Transform, Rigidbody2D* _pObject_RigidBody2D);
	virtual ~Physics();

	void PhysicsCheck();
	void CirclePhysicsCheck();
	void AABBPhysicsCheck();
	void OBBPhysicsCheck();

	void SetTarget(Transform* _pTarget_Transform, Rigidbody2D* _pTarget_RigidBody2D, CollisionType _Col_Type);
	Transform* GetTransform();
	Rigidbody2D* GetRigidbody2D();


	virtual void Draw();
	virtual void Update();
	virtual void Release();
};

