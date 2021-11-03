#include "pch.h"
#include "DHDebugger.h"

DHDebugger::DHDebugger(DHDebugger_Mode _Debugger_Mode, bool _Use_Logger /*= true*/, const TCHAR* _Title_Name /*= "DHDEBUG"*/)
{
	if (Use_Mode_Thread != nullptr)
	{
		printf_s("[DHDebugger 생성자 오류] 이미 생성된 객체입니다. \n");
		return;
	}

	Current_Mode = _Debugger_Mode;
	Use_Logger = _Use_Logger;

	if (Use_Logger)
	{
		m_LogManager = new DHLogger(_Title_Name);
	}

	// 지정된 디버거 모드에 따라 동작하도록.
	switch (_Debugger_Mode)
	{
	case DHDebugger::DHDebugger_Mode::KeyBoard_Mode:
		Initialize_KeyBoard_Mode();
		break;
	case DHDebugger::DHDebugger_Mode::Timer_Mode:
		Initialize_Timer_Mode();
		break;
	}

	Sleep(0);
}

DHDebugger::~DHDebugger()
{
	/// 쓰레드가 생성되었다면 종료 대기.
	if (Use_Mode_Thread != nullptr)
	{
		Is_End = true;
		Use_Mode_Thread->join();

		/// 만약 측정에 설정된 시간이전에 작업이 다끝났으면 이때까지 측정된 데이터라도 출력해줌.

		// 모든 테스트 케이스에 대하여
		for (int i = 0; i < MAX_CASE_COUNT; i++)
		{
			// 해당데이터를 보여준다.
			Measurable_Test_Case_Print_Timer((Test_Case_Number)i);
		}
	}

	/// 로거가 생성되었다면
	if (Use_Mode_Thread != nullptr)
	{
		m_LogManager->Release();
	}

	/// 키IO 가 생성 되었다면
	if (Use_Mode_Thread != nullptr)
	{
		delete m_KeyIO;
	}
}

void DHDebugger::Clear_Test_Case(Test_Case_Number _Test_Case_Num)
{
	Test_Case[_Test_Case_Num].Total_Time = std::chrono::milliseconds::zero();
	InterlockedExchange((volatile LONG*)(&Test_Case[_Test_Case_Num].Test_Count), 0);
}

void DHDebugger::Measurable_Test_Case_Print_KeyBoard(Test_Case_Number _Test_Case_Num)
{
	// 이미 측정을 하고 있는경우
	if (Test_Case[_Test_Case_Num].Measurable & MEASURABLE)
	{
		// 측정을 종료했다는 의미로 플래그를 꺼준다음
		InterlockedAnd((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), ~MEASURABLE);

		// 테스트를 1번이상 진행을 했다면
		if (Test_Case[_Test_Case_Num].Test_Count > 0)
		{
			// 콘솔 화면을 한번 지운 후에 결과 값을 보여준다. (전체 측정이 아닌경우 화면을 지우고 보여줌.)
			if (!Is_Total_Measure)
			{
				system("cls");
			}
			printf_s("[DHDebugger] [CASE_0%d] 측정 결과\n", _Test_Case_Num + 1);
			printf_s("[DHDebugger] 총 측정 횟수 : %d \n", Test_Case[_Test_Case_Num].Test_Count);
			printf_s("[DHDebugger] 총 걸린 시간 : %f ms\n", (Test_Case[_Test_Case_Num].Total_Time.count() * 1000));
			printf_s("[DHDebugger] 평균 걸린 시간. : %f ms\n", ((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

			if (Use_Logger)
			{
				TCHAR CASE_NAME[128];
				_stprintf_s(CASE_NAME, _T("CASE_0%d"), _Test_Case_Num+1);
				TCHAR CASE_DATA[1024];
				_stprintf_s(CASE_DATA, _T("총 측정 횟수 : %d\t 총 걸린 시간 : %f ms\t 평균 걸린 시간 : %f ms"),
					Test_Case[_Test_Case_Num].Test_Count, (Test_Case[_Test_Case_Num].Total_Time.count() * 1000), 
					((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

				m_LogManager->WriteLog(CASE_NAME, CASE_DATA);
			}

			// 해당 테스트 케이스에 대한 측정 횟수와 총 시간은 초기화 한다.
			Clear_Test_Case(_Test_Case_Num);
		}
	}
	else
	{
		// 쓰레드 환경이기때문에 Start 를 처음에 불러오지 않을 수도 있기에, 여기에 기록을 해둠. (기록에 유의미한 영향을 미치지는 않을 것임.)
		Test_Case[_Test_Case_Num].Start_Time = std::chrono::system_clock::now();
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURABLE);

		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			printf_s("[DHDebugger] [CASE_0%d] 측정 시작\n", _Test_Case_Num + 1);
		}
	}
}

void DHDebugger::Measurable_Test_Case_Print_Timer(Test_Case_Number _Test_Case_Num)
{
	// 테스트를 1번이상 진행을 했다면
	if (Test_Case[_Test_Case_Num].Test_Count > 0)
	{
		printf_s("[DHDebugger] [CASE_0%d] 측정 결과\n", _Test_Case_Num + 1);
		printf_s("[DHDebugger] 총 측정 횟수 : %d \n", Test_Case[_Test_Case_Num].Test_Count);
		printf_s("[DHDebugger] 총 걸린 시간 : %f ms\n", (Test_Case[_Test_Case_Num].Total_Time.count() * 1000));
		printf_s("[DHDebugger] 평균 걸린 시간. : %f ms\n", ((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

		if (Use_Logger)
		{
			TCHAR CASE_NAME[128];
			_stprintf_s(CASE_NAME, _T("CASE_0%d"), _Test_Case_Num + 1);
			TCHAR CASE_DATA[1024];
			_stprintf_s(CASE_DATA, _T("총 측정 횟수 : %d\t 총 걸린 시간 : %f ms\t 평균 걸린 시간 : %f ms"),
				Test_Case[_Test_Case_Num].Test_Count, (Test_Case[_Test_Case_Num].Total_Time.count() * 1000),
				((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

			m_LogManager->WriteLog(CASE_NAME, CASE_DATA);
		}

		// 해당 테스트 케이스에 대한 측정 횟수와 총 시간은 초기화 한다.
		Clear_Test_Case(_Test_Case_Num);
	}
}

void DHDebugger::Start_Mark_Timer(Test_Case_Number _Test_Case_Num)
{
	/// 측정을 시작하지 않은 상태라면 플래그를 켜준다.
	if ((Test_Case[_Test_Case_Num].Measurable & MEASURE_START) == 0)
	{
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURE_START);
	}

	switch (Current_Mode)
	{
	case DHDebugger::DHDebugger_Mode::KeyBoard_Mode:
	{
		// 측정 가능한 상태라면.
		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			// 현재 시간을 저장.
			Test_Case[_Test_Case_Num].Start_Time = std::chrono::system_clock::now();
		}
	}
		break;
	case DHDebugger::DHDebugger_Mode::Timer_Mode:
	{
		// 시작과 종료가 설정되어 있다면
		if (((Test_Case[_Test_Case_Num].Measurable & MEASURE_START) != 0) && ((Test_Case[_Test_Case_Num].Measurable & MEASURE_END) != 0))
		{
			Test_Case[_Test_Case_Num].Start_Time = std::chrono::system_clock::now();
		}
	}
		break;
	}

}

void DHDebugger::End_Mark_Timer(Test_Case_Number _Test_Case_Num)
{
	switch (Current_Mode)
	{
	case DHDebugger::DHDebugger_Mode::KeyBoard_Mode:
	{
		// 측정 가능한 상태라면.
		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			// 현재 시간에서 시작한 시간을 빼서 저장해둔다.
			Test_Case[_Test_Case_Num].Total_Time +=
				std::chrono::system_clock::now() - Test_Case[_Test_Case_Num].Start_Time;
			// 한번의 테스트 횟수가 완료 되었으므로 카운트를 증가시킨다.
			InterlockedIncrement((volatile LONG*)(&Test_Case[_Test_Case_Num].Test_Count));
			// ++Test_Case[_Test_Case_Num].Test_Count;
		}
	}
	break;
	case DHDebugger::DHDebugger_Mode::Timer_Mode:
	{
		// 시작과 종료가 설정되어 있다면
		if (((Test_Case[_Test_Case_Num].Measurable & MEASURE_START) != 0) && ((Test_Case[_Test_Case_Num].Measurable & MEASURE_END) != 0))
		{
			// 현재 시간에서 시작한 시간을 빼서 저장해둔다.
			Test_Case[_Test_Case_Num].Total_Time +=
				std::chrono::system_clock::now() - Test_Case[_Test_Case_Num].Start_Time;
			// 한번의 테스트 횟수가 완료 되었으므로 카운트를 증가시킨다.
			InterlockedIncrement((volatile LONG*)(&Test_Case[_Test_Case_Num].Test_Count));
		}
	}
	break;
	}

	/// 측정을 종료 플래그를 설정하지 않은 상태라면 플래그를 켜준다.
	if ((Test_Case[_Test_Case_Num].Measurable & MEASURE_END) == 0)
	{
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURE_END);
	}
}

void DHDebugger::Key_Input_Thread()
{
	while (!Is_End)
	{
		/// 테스트케이스 1~3번.
		///	1. 비트 플래그의 첫번째 자리가 1인경우를 체크 => & 연산.
		/// 2. & 연산의 결과가 1보다 크다면, 해당 플래그가 켜져 있음을 나타냄.
		/// 3-1. 플래그가 켜져 있다면 꺼주고
		/// 3-2. 플래그가 꺼져 있다면 켜준다.
		if (m_KeyIO->Key_Down(VK_F1))
		{
			Measurable_Test_Case_Print_KeyBoard(Test_Case_Number::CASE_01);
		}

		if (m_KeyIO->Key_Down(VK_F2))
		{
			Measurable_Test_Case_Print_KeyBoard(Test_Case_Number::CASE_02);
		}

		if (m_KeyIO->Key_Down(VK_F3))
		{
			Measurable_Test_Case_Print_KeyBoard(Test_Case_Number::CASE_03);
		}

		/// 로그 남기기 시작.
		if (m_KeyIO->Key_Down(VK_F4))
		{
			/// 불필요한 쓰레드 생성을 방지하기 위해 처음 생성자에서 부터 로거를 만들진 않는다.
			if (m_LogManager == nullptr)
			{
				m_LogManager = new DHLogger;
			}
			/// 로거 종료.
			else
			{
				m_LogManager->Release();
				m_LogManager = nullptr;
			}
		}

		/// 테스트 케이스 1~N개의 측정 시작.
		if (m_KeyIO->Key_Down(VK_F5))
		{
			// 전체 측정 시작
			if (Is_Total_Measure)
			{
				system("cls");
				// 모든 테스트 케이스를 측정을 한다는 플래그 설정.
				for (int i = 0; i < MAX_CASE_COUNT; i++)
				{
					Measurable_Test_Case_Print_KeyBoard((Test_Case_Number)i);
					//InterlockedOr((volatile LONG*)(&Test_Case[i].Measurable), MEASURABLE);
					//Test_Case[i].Measurable |= MEASURABLE;
				}

				InterlockedExchange((volatile LONG*)&Is_Total_Measure, !Is_Total_Measure);
				//Is_Total_Measure = !Is_Total_Measure;
			}
			// 전체 측정 종료
			else
			{
				// 모든 테스트 케이스를 측정하지 않는다는 플래그 설정.
				for (int i = 0; i < MAX_CASE_COUNT; i++)
				{
					Measurable_Test_Case_Print_KeyBoard((Test_Case_Number)i);
					//InterlockedAnd((volatile LONG*)(&Test_Case[i].Measurable), ~MEASURABLE);
				}

				InterlockedExchange((volatile LONG*)&Is_Total_Measure, !Is_Total_Measure);
				//Is_Total_Measure = !Is_Total_Measure;
			}
		}

		Sleep(0);
	}
}

void DHDebugger::Timer_Thread()
{
	Check_Start_Time = std::chrono::system_clock::now();

	while (!Is_End)
	{
		Current_Time = std::chrono::system_clock::now();
		Pass_Time += Current_Time - Check_Start_Time;
		Check_Start_Time = Current_Time;

		// 설정된 시간이 지났으면 로그를 기록한다.
		if ((Pass_Time.count() * 1000) > Current_Set_TimerSec)
		{
			Pass_Time = std::chrono::milliseconds::zero();

			// 모든 테스트 케이스에 대하여
			for (int i = 0; i < MAX_CASE_COUNT; i++)
			{
				// 해당데이터를 보여준다.
				Measurable_Test_Case_Print_Timer((Test_Case_Number)i);
			}
		}
	}
}

void DHDebugger::Initialize_KeyBoard_Mode()
{
	m_KeyIO = new DHKeyIO;
	/// 키보드를 통한 디버깅 모드에서 사용되는 쓰레드.
	Use_Mode_Thread = new std::thread(std::bind(&DHDebugger::Key_Input_Thread, this));
}

void DHDebugger::Initialize_Timer_Mode()
{
	/// 타이머를 통한 디버깅 모드에서 사용되는 쓰레드.
	Use_Mode_Thread = new std::thread(std::bind(&DHDebugger::Timer_Thread, this));
}

void DHDebugger::WriteLog(const TCHAR* _Log_Tag, const TCHAR* _Write_Data, ...)
{
	if (m_LogManager != nullptr)
	{
		TCHAR pStr[MAX_WORD];

		va_list args;
		va_start(args, _Write_Data);
		_vsntprintf_s(pStr, sizeof(pStr) / sizeof(TCHAR), sizeof(pStr), _Write_Data, args);
		va_end(args);

		m_LogManager->WriteLog(_Log_Tag, pStr);
	}
}

void DHDebugger::SetTimerSec(float _Timer_Sec)
{
	// 타이머 설정 (몇초마다 로그를 남길지?)
	if (_Timer_Sec < 0)
	{
		printf_s("[DHDebugger 타이머 셋팅오류] %f 초로 올바르지않은 숫자로 설정되었습니다. 기본값인 5초로 바뀝니다. \n", _Timer_Sec);
		Current_Set_TimerSec = 5000.f;
	}
	else
	{
		Current_Set_TimerSec = _Timer_Sec * 1000;
	}
}
