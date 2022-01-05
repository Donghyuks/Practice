#pragma once
#include <vector>

class Transform;
class Animation;
class Component;
class D2DEngine;

class Object
{
private:
	D2DEngine* pthEngine;
	Transform* pTransform;
	vector<Animation*> pAnimation;

	vector<Component*> thComponent;

private:
	bool Is_Debug = true;
	bool pre_collision = false;
	int Current_Ani_Num;

public:
	Object(D2DEngine* _thEngine, float _PosX, float _PosY, float _Angle, float _ScaleX = 1, float _ScaleY = 1);
	virtual ~Object();

	virtual void Draw();
	virtual void Update();

	virtual Transform* GetTransform();
	virtual vector<Component*> GetComponent();

	/// ������Ʈ ���̴� �Լ���..
	virtual void SetCollision(float _ColSizeX, float _ColSizeY = 1);
	virtual void SetPhysics(float _Mass = 1, float _Coefficient_Friction = 0.01);

	virtual void AddAnimation(int _Image_Cut_Count, float _Ani_Time, string _Image_File_Path, string _Image_Data = "");
	virtual void SetAnimationNum(int num);
	
	/// ���߿� �Ŵ����� ���� �Ǵ� �Լ���..

	/// <summary>
	/// ��� Object�� �� Object ���� �浹üũ.
	/// </summary>
	/// <param name="_Target"> �浹üũ�� �˻��ϰ� ���� Ÿ�� </param>
	/// <returns> �浹���� ��ȯ. �浹 => true / �浹�ƴ� => false </returns>
	virtual bool CheckCollision(Object* _Target, CollisionType _Col_Type = CollisionType::AABB);

public:
	void SetDebug(bool _Set_TF);
};

