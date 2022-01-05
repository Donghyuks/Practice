#pragma once

class D2DEngine;
class Transform;
class Rigidbody2D;

class Physics : public Component
{
private:
	D2DEngine* pthEngine;

	// ���� ������Ʈ�� ���� transform
	Transform* Object_transform;
	// Ÿ�� ������Ʈ�� ���� transform
	Transform* Target_transform;

	// ���� ������Ʈ�� ���� RigidBody
	Rigidbody2D* Object_Rigidbody2D;
	// Ÿ�� ������Ʈ�� ���� RigidBody
	Rigidbody2D* Target_Rigidbody2D;

	// �ݸ��� Ÿ�Կ� ���� ���������� �޶��� ���� �ִ�.
	CollisionType Col_Type;
	// �浹 �������. (�սǷ�)
	float Crash_Friction;
	// ź�� ���
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

