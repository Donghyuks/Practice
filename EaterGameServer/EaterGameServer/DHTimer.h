#pragma once
#include <chrono>

class DHTimer
{
private:
	unsigned int m_FPS = 60;
	double m_Frame_To_Second = 0.f;
		
	std::chrono::time_point<std::chrono::system_clock> Prev_Time;							// 지난 프레임 시간
	std::chrono::time_point<std::chrono::system_clock> Current_Time;						// 현재 프레임 시간
	std::chrono::duration<double> Passed_Time = std::chrono::duration<double>::zero();		// 지난시간
public:
	DHTimer();
	~DHTimer();

	void SetFrame(unsigned int _FPS);
	// true 일때 진행하면됨.
	bool Ready_Frame();
};

