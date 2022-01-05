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
	// D2D �׷��� ���ҽ� ����
	// D2D ���丮
	ID2D1Factory* pFactory;
	// D2D �ؽ�Ʈ ���丮
	IDWriteFactory* pWriteFactory;
	// ���� ȭ�鿡 �׷��� ������ Ÿ��.
	ID2D1HwndRenderTarget* pRenderTarget;

	// WIC ����
	// WIC �̹��� ���丮.
	IWICImagingFactory* pWICFactory;
	// ���� ��ȯ.
	IWICFormatConverter* pConvertedSrcBmp;
	// ���ڴ� ����.
	IWICBitmapDecoder* pDecodePtr;
	// ���ڴ��� ������ ȹ���� ����.
	IWICBitmapFrameDecode* pFramePtr;

	// ���� ������ �ڵ��� �޾ƿ� ������ �� ����.
	HWND g_hwnd;

private:
	// ���� ������ �귯�÷� ������ ����.
	ID2D1SolidColorBrush* pOld_Brush;
	// ���� �׸��� ���Ǵ� �귯��. ��� �Ŀ� ���� �귯�÷� ��������.
	ID2D1SolidColorBrush* pCurrent_Brush;
	// �׸��� ���� �귯��.
	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;
	// �ؽ�Ʈ ����
	IDWriteTextFormat* pTextFormat;

private:
	// �׸��� ��� �� on/off ����.
	bool Is_Grid_On = false;

public:
	D2DEngine();
	~D2DEngine();

	/// <summary>
	/// �ʱ�ȭ �۾�
	/// </summary>
	/// <param name="_hwnd"> ���� �����쿡 ���� �ڵ��� �޾� �����ص�. </param>
	void Initialize(HWND _hwnd);
	/// �׷��� ���ҽ� �ʱ�ȭ �� ����.
	void CreateGraphicsResources();
	/// �׷��� ���ҽ� ����.
	void DiscardGraphicsResources();
	/// Rendering ����
	void StartRender();
	/// Rendering ��
	void EndRender();

public:
	/// ���� ������â ũ�⿡ ���缭 Render Target �� ����� �����ϴ� �Լ�.
	void Resize_Current_Window();
	
	void SetGridMod(bool Is_OnOff, float _Width, float _Height, D2D1::ColorF _Col);
public:
	/// <summary>
	/// �귯�� Į���� �����ϴ� �Լ� - pCurrent_Brush ���� �ٲٰ�, ���� ���� pOld_Brush�� ������ ��.
	/// </summary>
	/// <param name="_Col"> ���ϴ� �÷� �Է� </param>
	void thSetBrushColor(D2D1::ColorF _Col);

	void thCreateGridPatternBrush(ID2D1RenderTarget* _pRenderTarget, ID2D1BitmapBrush** _ppBitmapBrush, float _Rect_Width, float _Rect_Height, D2D1::ColorF _Col = D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f));
	/// �귯�ø� ������� �������� ����.
	void thReMoveBrush();

public:
	/// <summary>
	/// ��Ʈ�� �ε��Լ�
	/// </summary>
	/// <param name="_File_Path"> ��Ʈ�� �ҷ��� ��ġ </param>
	/// <param name="_Load_Bitmap"> ��Ʈ���� �ҷ��� ������ ID2DIBitmap �� ������ </param>
	void thLoadBitmap(string _File_Path, ID2D1Bitmap** _Load_Bitmap);
	/// <summary>
	/// ��Ʈ�� �׸��� �Լ� 1 - ������ ũ�⸸ŭ �׸���.
	/// </summary>
	/// <param name="_Left"> �簢�� ���� ���� �� x ��ǥ </param>
	/// <param name="_Top"> �簢�� ���� ���� �� y ��ǥ </param>
	/// <param name="_Right"> �簢�� ���� ������ �Ʒ� x ��ǥ </param>
	/// <param name="_Bottom">�簢�� ���� ������ �Ʒ� y ��ǥ </param>
	/// <param name="_Load_Bitmap"> �׸� ��Ʈ�� </param>
	void thDrawBitmap(float _Left, float _Top, float _Right, float _Bottom, ID2D1Bitmap* _Load_Bitmap);
	/// <summary>
	/// ��Ʈ���� �׸��� �Լ� 2 - ���� ��ǥ�κ��� ��Ʈ���� ����ũ�⸸ŭ �׸���.
	/// </summary>
	/// <param name="_Start_PosX"> ���� X ��ǥ </param>
	/// <param name="_Start_PosY"> ���� Y ��ǥ </param>
	/// <param name="_Load_Bitmap"> �׸� ��Ʈ�� </param>
	void thDrawBitmap(float _Start_PosX, float _Start_PosY, ID2D1Bitmap* _Load_Bitmap);
	void thDrawTRSBitmap(D2D1_POINT_2F _Position, D2D1::Matrix3x2F _TRS_Matrix, ID2D1Bitmap* _Load_Bitmap);
	void thDrawBoundaryTRSBitmap(D2D1_POINT_2F _Position, D2D1_RECT_F _Cut_Boundary, D2D1::Matrix3x2F _TRS_Matrix, ID2D1Bitmap* _Load_Bitmap);
	/// <summary>
	/// �簢���� �׸��� �Լ� - ���� �� ���� ���⵵ ��������.
	/// </summary>
	/// <param name="_Left"> �簢�� ���� ���� �� x ��ǥ </param>
	/// <param name="_Top"> �簢�� ���� ���� �� y ��ǥ </param>
	/// <param name="_Right"> �簢�� ���� ������ �Ʒ� x ��ǥ </param>
	/// <param name="_Bottom">�簢�� ���� ������ �Ʒ� y ��ǥ </param>
	/// <param name="_Size"> ���� ���� </param>
	/// <param name="_Col"> �簢���� �׷��� ���� </param>
	void thDrawRectangle(float _Left, float _Top, float _Right, float _Bottom, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	void thDrawRectangle(float _Left, float _Top, float _Right, float _Bottom, float _Angle, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	Rotate_Point_Rect thRotationRect(float _Left, float _Top, float _Right, float _Bottom, float _Angle, float _Size = 1.0f);
	void thDrawFillRectangle(float _Left, float _Top, float _Right, float _Bottom, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// ���� �׸��� �Լ� - ���� �� ���� ���⵵ ��������.
	/// </summary>
	/// <param name="_x1"> �������� x ��ǥ </param>
	/// <param name="_y1"> �������� y ��ǥ </param>
	/// <param name="_x2"> ���������� x ��ǥ </param>
	/// <param name="_y2"> ���������� y ��ǥ </param>
	/// <param name="_Size"> ���� ���� </param>
	/// <param name="_Col"> ���� �׷��� ���� </param>
	void thDrawLine(float _x1, float _y1, float _x2, float _y2, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// ���� �׸��� �Լ� 1 - ������ �簢�� ������ ���� ���� �׸���.
	/// </summary>
	/// <param name="_Left"> �簢�� ���� ���� �� x ��ǥ </param>
	/// <param name="_Top"> �簢�� ���� ���� �� y ��ǥ </param>
	/// <param name="_Right"> �簢�� ���� ������ �Ʒ� x ��ǥ </param>
	/// <param name="_Bottom">�簢�� ���� ������ �Ʒ� y ��ǥ </param>
	/// <param name="_Size"> ���� ���� </param>
	/// <param name="_Col"> ���� �׷��� ���� </param>
	void thDrawEllipse(float _Left, float _Top, float _Right, float _Bottom, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// ���� �׸��� �Լ� 2 - ������ �������� �������� ���� �׸���.
	/// </summary>
	/// <param name="_x"> ������ x ��ǥ </param>
	/// <param name="_y"> ������ y ��ǥ </param>
	/// <param name="_Radius"> ���� ������ ���� </param>
	/// <param name="_Size"> ���� ���� </param>
	/// <param name="_Col"> ���� �׷��� ���� </param>
	void thDrawEllipse(float _x, float _y, float _Radius, float _Size = 1.0f, D2D1::ColorF _Col = D2D1::ColorF::Black);
	void thDrawFillEllipse(float _x, float _y, float _Radius, D2D1::ColorF _Col = D2D1::ColorF::Black);
	/// <summary>
	/// �ؽ�Ʈ ��� �Լ� - �������ڸ� ���� �� �ִ�. ���� ��������.
	/// </summary>
	/// <param name="_x"> ����� �ؽ�Ʈ�� ���� x ��ǥ </param>
	/// <param name="_y"> ����� �ؽ�Ʈ�� ���� y ��ǥ </param>
	/// <param name="_Col"> ����� �ؽ�Ʈ�� ���� </param>
	/// <param name="_Input_String"> ����� �ؽ�Ʈ </param>
	void thDrawText(int _x, int _y, D2D1::ColorF _Col, const wchar_t* _Input_String, ...);
};