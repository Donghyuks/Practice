#include "DHKeyIO.h"
#include <windows.h>

bool DHKeyIO::Key_Up(unsigned char Key_Input)
{
	/// 만약 입력된 키가 눌렸으면 현재 눌린상태로 플래그바꿈..
	if (GetAsyncKeyState(Key_Input))
	{
		m_Key_Flag[Key_Input].Key_Current_State = true;

	}
	else
	{
		m_Key_Flag[Key_Input].Key_Current_State = false;
	}

	/// 이전번에 눌렸던 상태고 이번에 키가 떼졌으면 ( 키를 뗏을 때 )
	if (m_Key_Flag[Key_Input].Key_Current_State == false &&
		m_Key_Flag[Key_Input].Key_Prev_State == true)
	{
		m_Key_Flag[Key_Input].Key_Prev_State = m_Key_Flag[Key_Input].Key_Current_State;
		return true;
	}

	/// 이전상태 저장.
	m_Key_Flag[Key_Input].Key_Prev_State = m_Key_Flag[Key_Input].Key_Current_State;
	return false;
}

bool DHKeyIO::Key_Down(unsigned char Key_Input)
{
	/// 만약 입력된 키가 눌렸으면 현재 눌린상태로 플래그바꿈..
	if (GetAsyncKeyState(Key_Input))
	{
		m_Key_Flag[Key_Input].Key_Current_State = true;

	}
	else
	{
		m_Key_Flag[Key_Input].Key_Current_State = false;
	}

	/// 만약 이번에 눌렸고 이전번에 눌리지않았으면 ( 키가 눌렸을 때 )
	if (m_Key_Flag[Key_Input].Key_Current_State == true &&
		m_Key_Flag[Key_Input].Key_Prev_State == false)
	{
		m_Key_Flag[Key_Input].Key_Prev_State = m_Key_Flag[Key_Input].Key_Current_State;
		return true;
	}

	/// 이전상태 저장.
	m_Key_Flag[Key_Input].Key_Prev_State = m_Key_Flag[Key_Input].Key_Current_State;
	return false;
}

bool DHKeyIO::Key_Press(unsigned char Key_Input)
{
	if (GetAsyncKeyState(Key_Input))
	{
		return true;
	}

	return false;
}
