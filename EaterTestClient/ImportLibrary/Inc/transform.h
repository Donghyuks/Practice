#pragma once

#include "thEngineHeader.h"

class Transform
{
private:
	// ÁßÁ¡ ÁÂÇ¥
	D2D1_POINT_2F position;
	D2D1_POINT_2F scale;
	float angle;

	// TRS
	D2D1_POINT_2F TRS_trans;
	float TRS_rotation;
	D2D1_POINT_2F TRS_scale;
	D2D1::Matrix3x2F TRS_Matrix;

public:
	Transform(D2D1_POINT_2F _Pos = { 0,0 }, D2D1_POINT_2F _Scale = { 1,1 }, float _Angle = 0, 
		D2D1_POINT_2F _TRS_trans = { 0,0 }, D2D1_POINT_2F _TRS_scale = { 0,0 }, float _TRS_rotation = 0);
	virtual ~Transform();

	Transform operator+=(const Transform& Another);

	virtual D2D1_POINT_2F GetPosition();
	virtual D2D1_POINT_2F GetScale();
	virtual float GetAngle();
	virtual D2D1_POINT_2F GetTRStrans();

	Vector2<float> GetVector();
	virtual float GetVelocity();
	virtual D2D1::Matrix3x2F GetMatrix();

	virtual void SetPosition(D2D1_POINT_2F _Pos);
	virtual void SetPosition(float _PosX, float _PosY);
	virtual void SetScale(D2D1_POINT_2F _Scale);
	virtual void SetScale(float _ScaleX, float _ScaleY);
	virtual void SetAngle(float _Angle);

	virtual void SetTRStrans(D2D1_POINT_2F _TRS_trans);
	virtual void SetTRStrans(float _TRS_transX, float _TRS_transY);
	virtual void SetTRSscale(D2D1_POINT_2F _TRS_scale);
	virtual void SetTRSscale(float _TRS_scaleX, float _TRS_scaleY);
	virtual void SetTRSrotation(float _TRS_rotation);

	virtual void ResetTRStrans();

	void SetMatrix();
	void Update();
};

