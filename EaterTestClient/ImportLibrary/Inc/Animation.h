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
	// ���� ������.
	D2DEngine* pthEngine;
	// Ʈ������ ������.
	Transform* Object_transform;

	// ����¥�� �ִϸ��̼� ������.
	ID2D1Bitmap* Animation_Bitmap;

	// �ִϸ��̼� ����� ������.
	vector<Frame_Data*> F_Data;
	
	// �� �� ������
	float One_Cut_SizeX;
	float One_Cut_SizeY;

	// �ִϸ��̼� �� ����
	int Image_Cut_Count;
	// �ִϸ��̼� ��� �ð� ( �ӵ��� ���� ).
	float Ani_Time;

	// �� �������� ����ɶ����� 1�� �����ϴ� ��� ������ ����.
	int Pass_Frame = 0;
	// �� �ƴ� ����� �ð�.
	float One_Cut_Time = 0;
	// ���� ��� �ִϸ��̼� ������.
	int Current_Frame = 0;

public:
	Animation(Transform* _Obj_Transform, D2DEngine* _pthEngine, int _Image_Cut_Count, float _Ani_Time, string _Image_File_Path, string _Image_Data = "");
	~Animation();

	virtual void Frame_Reset();

	virtual void Update();
	virtual void Draw();
	virtual void Release();
};

