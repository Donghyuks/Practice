#pragma once

#define CHANNEL_COUNT 256
#define DEFAULT_FREQUENCY 44100

#ifdef EATERSOUND_EXPORTS
#define EATERSOUND_DLL __declspec(dllexport)
#else
#define EATERSOUND_DLL __declspec(dllimport)
#endif

// CString 사용.
#include <atlstr.h>

// 사운드의 종류 지정.
enum class EATERSOUND_DLL Sound_Category
{
	BGM,	// BGM 종류 사운드
	SFX,	// SFX 종류 사운드
	Master	// 전체 사운드
};

// 사운드의 볼륨 / 프리퀀시 / 피치
struct EATERSOUND_DLL Sound_VFP
{
	// 사운드 볼륨
	float Volume	=	1.f;
	// 재생 빈도 (속도에 영향을 준다. 2배면 속도 2배와 같은효과이긴함.)
	float FreQuency =	44100.f;
	// 재생 속도
	float Pitch		=	1.f;
};