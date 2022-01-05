// 여기에 미리 컴파일하려는 헤더 추가

#ifndef PCH_H
#define PCH_H
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <windows.h>
#include <string>
#include <d2d1.h>
#include <tchar.h>
#include <wchar.h>
#include <dwrite.h>
#include <assert.h>
#include <vector>
#include <math.h>

#include "SingleTon.h"
#include "thEngineDefine.h"
#include "Vector2.h"
#include "Component.h"

// WIC 사용을 위함.
#include <wincodec.h>
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "d2d1")

using namespace std;
#endif //PCH_H