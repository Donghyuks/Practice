#pragma once

#include "thEngineHeader.h"

static struct Rotate_Point_Rect
{
	D2D1_POINT_2F LT = { 0.0f, 0.0f };
	D2D1_POINT_2F LB = { 0.0f, 0.0f };
	D2D1_POINT_2F RT = { 0.0f, 0.0f };
	D2D1_POINT_2F RB = { 0.0f, 0.0f };
};

class D2DEngine : public Singleton<D2DEngine>
{
private:
	// D2D 그래픽 리소스 관련
	// D2D 팩토리
	ID2D1Factory* pFactory;
	// D2D 텍스트 팩토리
	IDWriteFactory* pWriteFactory;
	// 실제 화면에 그려질 랜더링 타겟.
	ID2D1HwndRenderTarget* pRenderTarget;

	// WIC 관련
	// WIC 이미지 팩토리.
	IWICImagingFactory* pWICFactory;
	// 포맷 변환.
	IWICFormatConverter* pConvertedSrcBmp;
	// 디코더 생성.
	IWICBitmapDecoder* pDecodePtr;
	// 디코더의 프레임 획득을 위함.
	IWICBitmapFrameDecode* pFramePtr;

	// 메인 윈도우 핸들을 받아와 저장해 둘 변수.
	HWND g_hwnd;

private:
	// 원래 색깔의 브러시로 돌리기 위함.
	ID2D1SolidColorBrush* pOld_Brush;
	// 현재 그릴때 사용되는 브러시. 사용 후에 원래 브러시로 돌려놓음.
	ID2D1SolidColorBrush* pCurrent_Brush;
	// 그리드 패턴 브러시.
	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;
	// 텍스트 포멧
	IDWriteTextFormat* pTextFormat;

private:
	// 그리드 모드 의 on/off 여부.
	bool Is_Grid_On = false;

public:
	D2DEngine();
	~D2DEngine();

	/// <summary>
	/// 초기화 작업
	/// </summary>
	/// <param name="_hwnd"> 메인 윈도우에 대한 핸들을 받아 저장해둠. </param>
	void Initialize(HWND _hwnd);
	/// 그래픽 리소스 초기화 및 생성.
	void CreateGraphicsResources();
	/// 그래픽 리소스 제거.
	void DiscardGraphicsResources();
	/// Rendering 시작
	void StartRender();
	/// Rendering 끝
	void EndRender();

public:
	/// 현재 윈도우창 크기에 맞춰서 Render Target 의 사이즈를 변경하는 함수.
	void Resize_Current_Window();
	
	void SetGridMod(bool Is_OnOff, float _Width, float _Height, D2D1::ColorF _Col);
public:
	/// <summary>
	/// 브러시 칼러를 지정하는 함수 - pCurrent_Brush 값을 바꾸고, 기존 값을 pOld_Brush에 저장해 둠.
	/// </summary>
	/// <param name="_Col"> 원하는 컬러 입력 </param>
	void thSetBrushColor(D2D1::ColorF _Col);

	void thCreateGridPatternBrush(ID2D1RenderTarget* _pRenderTarget, ID2D1BitmapBrush** _ppBitmapBrush, float _Rect_Width, float _Rect_Height, D2D1::ColorF _Col = D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f));
	/// 브러시를 원래대로 돌려놓기 위함.
	void thReMoveBrush();

public:
	/// <summary>
	/// 비트맵 로딩함수
	/// </summary>
	/// <param name="_File_Path"> 비트맵 불러올 위치 </param>
	/// <param name="_Load_Bitmap"> 비트맵을 불러와 저장할 ID2DIBitmap 형 포인터 </param>
	void thLoadBitmap(string _File_Path, ID2D1Bitmap** _Load_Bitmap);
	/// <summary>
	/// 비트맵 그리는 함수 1 - 지정한 크기만큼 그린다.
	/// </summary>
	/// <param name="_Left"> 사각형 영역 왼쪽 위 x 좌표 </param>
	/// <param name="_Top"> 사각형 영역 왼쪽 위 y 좌표 </param>
	/// <param name="_Right"> 사각형 영역 오른쪽 아래 x 좌표 </param>
	/// <param name="_Bottom">사각형 영역 오른쪽 아래 y 좌표 </param>
	/// <param name="_Load_Bitmap"> 그릴 비트맵 </param>
	void thDrawBitmap(float _Left, float _Top, float _Right, float _Bottom, ID2D1Bitmap* _Load_Bitmap);
	/// <summary>
	/// 비트맵을 그리는 함수 2 - 지정 좌표로부터 비트맵의 원본크기만큼 그린다.
	/// </summary>
	/// <param name="_Start_PosX"> 시작 X 좌표 </param>
	/// <param name="_Start_PosY"> 시작 Y 좌표 </param>
	/// <param name="_Load_Bitmap"> 그릴 비트맵 </param>
	void thDrawBitmap(float _Start_PosX, float _Start_PosY, ID2D1Bitmap* _Load_Bitmap);
	void thDrawTRSBitmap(D2D1_POINT_2F _Position, D2D1::Matrix3x2F _TRS_Matrix, ID2D1Bitmap* _Load_Bitmap);
	void thDrawBoundaryTRSBitmap(D2D1_POINT_2F _Position, D2D1_RECT_F _Cut_Boundary, D2D1::Matrix3x2F _TRS_Matrix, ID2D1Bitmap* _Load_Bitmap);
	/// <summary>
	/// 사각형을 그리는 함수 - 색깔 및 선의 굵기도 지정가능.
	/// </summary>
	/// <param name="_Left"> 사각형 영역 왼쪽 위 x 좌표 </param>
	/// <param name="_Top"> 사각형 영역 왼쪽 위 y 좌표 </param>
	/// <param name="_Right"> 사각형 영역 오른쪽 아래 x 좌표 </param>
	/// <param name="_Bottom">사각형 영역 오른쪽 아래 y 좌표 </param>
	/// <param name="_Size"> 선의 굵기 </param>
	/// <param name="_Col"> 사각형이 그려질 색깔 </param>
	void thDrawRectangle(float _Left, float _Top, float _Right, float _Bottom, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	void thDrawRectangle(float _Left, float _Top, float _Right, float _Bottom, float _Angle, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	Rotate_Point_Rect thRotationRect(float _Left, float _Top, float _Right, float _Bottom, float _Angle, float _Size = 1.0f);
	void thDrawFillRectangle(float _Left, float _Top, float _Right, float _Bottom, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// 선을 그리는 함수 - 색깔 및 선의 굵기도 지정가능.
	/// </summary>
	/// <param name="_x1"> 시작점의 x 좌표 </param>
	/// <param name="_y1"> 시작점의 y 좌표 </param>
	/// <param name="_x2"> 끝나는점의 x 좌표 </param>
	/// <param name="_y2"> 끝나는점의 y 좌표 </param>
	/// <param name="_Size"> 선의 굵기 </param>
	/// <param name="_Col"> 선이 그려질 색깔 </param>
	void thDrawLine(float _x1, float _y1, float _x2, float _y2, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// 원을 그리는 함수 1 - 지정한 사각형 영역에 맞춰 원을 그린다.
	/// </summary>
	/// <param name="_Left"> 사각형 영역 왼쪽 위 x 좌표 </param>
	/// <param name="_Top"> 사각형 영역 왼쪽 위 y 좌표 </param>
	/// <param name="_Right"> 사각형 영역 오른쪽 아래 x 좌표 </param>
	/// <param name="_Bottom">사각형 영역 오른쪽 아래 y 좌표 </param>
	/// <param name="_Size"> 선의 굵기 </param>
	/// <param name="_Col"> 원이 그려질 색깔 </param>
	void thDrawEllipse(float _Left, float _Top, float _Right, float _Bottom, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// 원을 그리는 함수 2 - 중점과 반지름을 기준으로 원을 그린다.
	/// </summary>
	/// <param name="_x"> 중점의 x 좌표 </param>
	/// <param name="_y"> 중점의 y 좌표 </param>
	/// <param name="_Radius"> 원의 반지름 길이 </param>
	/// <param name="_Size"> 선의 굵기 </param>
	/// <param name="_Col"> 원이 그려질 색깔 </param>
	void thDrawEllipse(float _x, float _y, float _Radius, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	void thDrawFillEllipse(float _x, float _y, float _Radius, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// 텍스트 출력 함수 - 가변인자를 받을 수 있다. 색깔도 지정가능.
	/// </summary>
	/// <param name="_x"> 출력할 텍스트의 기준 x 좌표 </param>
	/// <param name="_y"> 출력할 텍스트의 기준 y 좌표 </param>
	/// <param name="_Col"> 출력할 텍스트의 색깔 </param>
	/// <param name="_Input_String"> 출력할 텍스트 </param>
	void thDrawText(int _x, int _y, D2D1::ColorF _Col, const wchar_t* _Input_String, ...);
};