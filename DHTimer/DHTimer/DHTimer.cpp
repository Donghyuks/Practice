#include "DHTimer.h"

DHTimer::DHTimer()
{
	Prev_Time = std::chrono::system_clock::now();
	Start_Time = std::chrono::system_clock::now();
	Current_Time = std::chrono::system_clock::now();
}

DHTimer::~DHTimer()
{

}

void DHTimer::Update()
{
	Current_Time = std::chrono::system_clock::now();

	for (int i = 0; i < TIMER_COUNT; i++)
	{
		m_Timer[i].Passed_Time += (Current_Time - Prev_Time);
	}

	Prev_Time = Current_Time;
}

bool DHTimer::SetFrame(unsigned int _FPS, TIMER_NUM _Using_Timer /*= TIMER_01*/)
{
	if (_Using_Timer >= TIMER_COUNT) return false;

	m_Timer[_Using_Timer].Check_Cycle_Sec = (double)1 / _FPS;

	return true;
}

bool DHTimer::SetTime(double _Sec, TIMER_NUM _Using_Timer /*= TIMER_01*/)
{
	if (_Using_Timer >= TIMER_COUNT) return false;

	m_Timer[_Using_Timer].Check_Cycle_Sec = _Sec;

	return true;
}

bool DHTimer::Ready_Frame(TIMER_NUM _Using_Timer /*= TIMER_01*/)
{
	if (m_Timer[_Using_Timer].Passed_Time.count() > m_Timer[_Using_Timer].Check_Cycle_Sec)
	{
		m_Timer[_Using_Timer].Passed_Time = std::chrono::duration<double>::zero();
		return true;
	}

	return false;
}

double DHTimer::GetTotalPassedTimeSec()
{
	std::chrono::time_point<std::chrono::system_clock> _Current_Time = std::chrono::system_clock::now();
	Total_Passed_Time = _Current_Time - Start_Time;

	return Total_Passed_Time.count();
}
