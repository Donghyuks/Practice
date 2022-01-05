#pragma once

#include "Component.h"

class Transform;

class Rigidbody2D : public Component
{
private:
	// Æ®·£½ºÆû.
	Transform* Object_Transform;

	// ¹Ý»ç º¤ÅÍ
	Vector2<float> Object_Reflection;
	// Áú·®
	float Mass;
	// ¸¶Âû °è¼ö.
	float Coefficient_Friction;

public:
	Rigidbody2D(Transform* _Object_Transform, float _Mass = 1, float _Coefficient_Friction = 0.01);
	virtual ~Rigidbody2D();

	void SetReflectionVector(Vector2<float> _Reflection_Vector);
	Vector2<float> GetReflectionVector();

	float GetMass();
	float GetCofficientFriction();

	virtual void Draw();
	virtual void Update();
	virtual void Release();
};

