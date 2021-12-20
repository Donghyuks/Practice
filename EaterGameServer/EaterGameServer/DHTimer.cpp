#include "DHTimer.h"

DHTimer::DHTimer()
{
	m_Frame_To_Second = (double)1 / m_FPS;
	Prev_Time = std::chrono::system_clock::now();
}

DHTimer::~DHTimer()
{

}

void DHTimer::SetFrame(unsigned int _FPS)
{
	m_FPS = _FPS;
	m_Frame_To_Second = (double)1 / m_FPS;
}

bool DHTimer::Ready_Frame()
{
	Current_Time = std::chrono::system_clock::now();
	Passed_Time += (Current_Time - Prev_Time);
	Prev_Time = Current_Time;

	if (Passed_Time.count() > m_Frame_To_Second)
	{
		Passed_Time = std::chrono::duration<double>::zero();
		return true;
	}

	return false;
}
