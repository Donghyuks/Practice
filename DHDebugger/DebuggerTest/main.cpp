
#include "DHDebugger.h"

int main()
{
	int k = 0;
	/// Ÿ�̸� ���
	DHDebugger my_Debugger(DHDebugger::DHDebugger_Mode::Timer_Mode);
	my_Debugger.SetTimerSec(2);

	/// Ű���� ���
	//DHDebugger my_Debugger(DHDebugger::DHDebugger_Mode::KeyBoard_Mode);

	my_Debugger.WriteLog(_T("�׽�Ʈ"), _T("%d ����"), 20);
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