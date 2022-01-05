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

	/// 컴포넌트 붙이는 함수들..
	virtual void SetCollision(float _ColSizeX, float _ColSizeY = 1);
	virtual void SetPhysics(float _Mass = 1, float _Coefficient_Friction = 0.01);

	virtual void AddAnimation(int _Image_Cut_Count, float _Ani_Time, string _Image_File_Path, string _Image_Data = "");
	virtual void SetAnimationNum(int num);
	
	/// 나중에 매니저로 뺴도 되는 함수들..

	/// <summary>
	/// 대상 Object와 현 Object 간의 충돌체크.
	/// </summary>
	/// <param name="_Target"> 충돌체크를 검사하고 싶은 타겟 </param>
	/// <returns> 충돌여부 반환. 충돌 => true / 충돌아님 => false </returns>
	virtual bool CheckCollision(Object* _Target, CollisionType _Col_Type = CollisionType::AABB);

public:
	void SetDebug(bool _Set_TF);
};

