#pragma once

#include "thEngineHeader.h"

class Frame_Data
{
public:
	// �������ӿ� �ش��ϴ� ��Ʈ�� ���� ��ġ.
	float Draw_PosX;
	float Draw_PosY;

	// �ش� �����ӿ� �Ҵ�� �ӵ� (��� ��)
	float Frame_Speed;

	/// �߰��� �� ��Ÿ �Ӽ��������� ���⿡ ����..
	

public:
	Frame_Data(float _Draw_PosX, float _Draw_PosY, float _Frame_Speed = 1);
	~Frame_Data();

};
