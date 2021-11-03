
#include "DHDebugger.h"

int main()
{
	int k = 0;
	/// 타이머 모드
	DHDebugger my_Debugger(DHDebugger::DHDebugger_Mode::Timer_Mode);
	my_Debugger.SetTimerSec(2);

	/// 키보드 모드
	//DHDebugger my_Debugger(DHDebugger::DHDebugger_Mode::KeyBoard_Mode);

	my_Debugger.WriteLog(_T("테스트"), _T("%d 숫자"), 20);
	while (true)
	{
		my_Debugger.Start_Mark_Timer(DHDebugger::CASE_01);

		for (int i = 0; i < 1000; i++)
		{
			k += 1;
		}

		my_Debugger.End_Mark_Timer(DHDebugger::CASE_01);
	}

	return true;
}