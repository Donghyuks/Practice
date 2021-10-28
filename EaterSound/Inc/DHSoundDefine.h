#pragma once

#define CHANNEL_COUNT 256
#define DEFAULT_FREQUENCY 44100

#ifdef EATERSOUND_EXPORTS
#define EATERSOUND_DLL __declspec(dllexport)
#else
#define EATERSOUND_DLL __declspec(dllimport)
#endif

// CString ���.
#include <atlstr.h>

// ������ ���� ����.
enum class EATERSOUND_DLL Sound_Category
{
	BGM,	// BGM ���� ����
	SFX,	// SFX ���� ����
	Master	// ��ü ����
};

// ������ ���� / �������� / ��ġ
struct EATERSOUND_DLL Sound_VFP
{
	// ���� ����
	float Volume	=	1.f;
	// ��� �� (�ӵ��� ������ �ش�. 2��� �ӵ� 2��� ����ȿ���̱���.)
	float FreQuency =	44100.f;
	// ��� �ӵ�
	float Pitch		=	1.f;
};