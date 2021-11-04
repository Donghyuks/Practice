#pragma once

#ifdef DHKEYIO_EXPORTS
	#define DHKEY_DLL __declspec(dllexport)
#else
	#define DHKEY_DLL __declspec(dllimport)
#endif

struct DHKEY_DLL Key_Flag
{
	bool Key_Current_State = false;
	bool Key_Prev_State = false;
};

class DHKEY_DLL DHKeyIO
{
private:
	bool Keeping = false;	// Ű�� ������ �������� üũ.
public:
	Key_Flag m_Key_Flag[256];

	/// Ű�� ������ ���� ��.
	bool Key_Up(unsigned char Key_Input);
	/// Ű�� ó�� �������� ��.
	bool Key_Down(unsigned char Key_Input);
	/// Ű�� ������ ���� ��.
	bool Key_Press(unsigned char Key_Input);
};

