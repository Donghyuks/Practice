#include "pch.h"
#include "DHDebugger.h"

DHDebugger::DHDebugger(DHDebugger_Mode _Debugger_Mode, bool _Use_Logger /*= true*/, const TCHAR* _Title_Name /*= "DHDEBUG"*/)
{
	if (Use_Mode_Thread != nullptr)
	{
		printf_s("[DHDebugger ������ ����] �̹� ������ ��ü�Դϴ�. \n");
		return;
	}

	Current_Mode = _Debugger_Mode;
	Use_Logger = _Use_Logger;

	if (Use_Logger)
	{
		m_LogManager = new DHLogger(_Title_Name);
	}

	// ������ ����� ��忡 ���� �����ϵ���.
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
	/// �����尡 �����Ǿ��ٸ� ���� ���.
	if (Use_Mode_Thread != nullptr)
	{
		Is_End = true;
		Use_Mode_Thread->join();

		/// ���� ������ ������ �ð������� �۾��� �ٳ������� �̶����� ������ �����Ͷ� �������.

		// ��� �׽�Ʈ ���̽��� ���Ͽ�
		for (int i = 0; i < MAX_CASE_COUNT; i++)
		{
			// �ش絥���͸� �����ش�.
			Measurable_Test_Case_Print_Timer((Test_Case_Number)i);
		}
	}

	/// �ΰŰ� �����Ǿ��ٸ�
	if (Use_Mode_Thread != nullptr)
	{
		m_LogManager->Release();
	}

	/// ŰIO �� ���� �Ǿ��ٸ�
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
	// �̹� ������ �ϰ� �ִ°��
	if (Test_Case[_Test_Case_Num].Measurable & MEASURABLE)
	{
		// ������ �����ߴٴ� �ǹ̷� �÷��׸� ���ش���
		InterlockedAnd((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), ~MEASURABLE);

		// �׽�Ʈ�� 1���̻� ������ �ߴٸ�
		if (Test_Case[_Test_Case_Num].Test_Count > 0)
		{
			// �ܼ� ȭ���� �ѹ� ���� �Ŀ� ��� ���� �����ش�. (��ü ������ �ƴѰ�� ȭ���� ����� ������.)
			if (!Is_Total_Measure)
			{
				system("cls");
			}
			printf_s("[DHDebugger] [CASE_0%d] ���� ���\n", _Test_Case_Num + 1);
			printf_s("[DHDebugger] �� ���� Ƚ�� : %d \n", Test_Case[_Test_Case_Num].Test_Count);
			printf_s("[DHDebugger] �� �ɸ� �ð� : %f ms\n", (Test_Case[_Test_Case_Num].Total_Time.count() * 1000));
			printf_s("[DHDebugger] ��� �ɸ� �ð�. : %f ms\n", ((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

			if (Use_Logger)
			{
				TCHAR CASE_NAME[128];
				_stprintf_s(CASE_NAME, _T("CASE_0%d"), _Test_Case_Num+1);
				TCHAR CASE_DATA[1024];
				_stprintf_s(CASE_DATA, _T("�� ���� Ƚ�� : %d\t �� �ɸ� �ð� : %f ms\t ��� �ɸ� �ð� : %f ms"),
					Test_Case[_Test_Case_Num].Test_Count, (Test_Case[_Test_Case_Num].Total_Time.count() * 1000), 
					((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

				m_LogManager->WriteLog(CASE_NAME, CASE_DATA);
			}

			// �ش� �׽�Ʈ ���̽��� ���� ���� Ƚ���� �� �ð��� �ʱ�ȭ �Ѵ�.
			Clear_Test_Case(_Test_Case_Num);
		}
	}
	else
	{
		// ������ ȯ���̱⶧���� Start �� ó���� �ҷ����� ���� ���� �ֱ⿡, ���⿡ ����� �ص�. (��Ͽ� ���ǹ��� ������ ��ġ���� ���� ����.)
		Test_Case[_Test_Case_Num].Start_Time = std::chrono::system_clock::now();
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURABLE);

		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			printf_s("[DHDebugger] [CASE_0%d] ���� ����\n", _Test_Case_Num + 1);
		}
	}
}

void DHDebugger::Measurable_Test_Case_Print_Timer(Test_Case_Number _Test_Case_Num)
{
	// �׽�Ʈ�� 1���̻� ������ �ߴٸ�
	if (Test_Case[_Test_Case_Num].Test_Count > 0)
	{
		printf_s("[DHDebugger] [CASE_0%d] ���� ���\n", _Test_Case_Num + 1);
		printf_s("[DHDebugger] �� ���� Ƚ�� : %d \n", Test_Case[_Test_Case_Num].Test_Count);
		printf_s("[DHDebugger] �� �ɸ� �ð� : %f ms\n", (Test_Case[_Test_Case_Num].Total_Time.count() * 1000));
		printf_s("[DHDebugger] ��� �ɸ� �ð�. : %f ms\n", ((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

		if (Use_Logger)
		{
			TCHAR CASE_NAME[128];
			_stprintf_s(CASE_NAME, _T("CASE_0%d"), _Test_Case_Num + 1);
			TCHAR CASE_DATA[1024];
			_stprintf_s(CASE_DATA, _T("�� ���� Ƚ�� : %d\t �� �ɸ� �ð� : %f ms\t ��� �ɸ� �ð� : %f ms"),
				Test_Case[_Test_Case_Num].Test_Count, (Test_Case[_Test_Case_Num].Total_Time.count() * 1000),
				((Test_Case[_Test_Case_Num].Total_Time.count() * 1000) / (double)Test_Case[_Test_Case_Num].Test_Count));

			m_LogManager->WriteLog(CASE_NAME, CASE_DATA);
		}

		// �ش� �׽�Ʈ ���̽��� ���� ���� Ƚ���� �� �ð��� �ʱ�ȭ �Ѵ�.
		Clear_Test_Case(_Test_Case_Num);
	}
}

void DHDebugger::Start_Mark_Timer(Test_Case_Number _Test_Case_Num)
{
	/// ������ �������� ���� ���¶�� �÷��׸� ���ش�.
	if ((Test_Case[_Test_Case_Num].Measurable & MEASURE_START) == 0)
	{
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURE_START);
	}

	switch (Current_Mode)
	{
	case DHDebugger::DHDebugger_Mode::KeyBoard_Mode:
	{
		// ���� ������ ���¶��.
		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			// ���� �ð��� ����.
			Test_Case[_Test_Case_Num].Start_Time = std::chrono::system_clock::now();
		}
	}
		break;
	case DHDebugger::DHDebugger_Mode::Timer_Mode:
	{
		// ���۰� ���ᰡ �����Ǿ� �ִٸ�
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
		// ���� ������ ���¶��.
		if (Test_Case[_Test_Case_Num].Measurable == MEASURABLE_STAT)
		{
			// ���� �ð����� ������ �ð��� ���� �����صд�.
			Test_Case[_Test_Case_Num].Total_Time +=
				std::chrono::system_clock::now() - Test_Case[_Test_Case_Num].Start_Time;
			// �ѹ��� �׽�Ʈ Ƚ���� �Ϸ� �Ǿ����Ƿ� ī��Ʈ�� ������Ų��.
			InterlockedIncrement((volatile LONG*)(&Test_Case[_Test_Case_Num].Test_Count));
			// ++Test_Case[_Test_Case_Num].Test_Count;
		}
	}
	break;
	case DHDebugger::DHDebugger_Mode::Timer_Mode:
	{
		// ���۰� ���ᰡ �����Ǿ� �ִٸ�
		if (((Test_Case[_Test_Case_Num].Measurable & MEASURE_START) != 0) && ((Test_Case[_Test_Case_Num].Measurable & MEASURE_END) != 0))
		{
			// ���� �ð����� ������ �ð��� ���� �����صд�.
			Test_Case[_Test_Case_Num].Total_Time +=
				std::chrono::system_clock::now() - Test_Case[_Test_Case_Num].Start_Time;
			// �ѹ��� �׽�Ʈ Ƚ���� �Ϸ� �Ǿ����Ƿ� ī��Ʈ�� ������Ų��.
			InterlockedIncrement((volatile LONG*)(&Test_Case[_Test_Case_Num].Test_Count));
		}
	}
	break;
	}

	/// ������ ���� �÷��׸� �������� ���� ���¶�� �÷��׸� ���ش�.
	if ((Test_Case[_Test_Case_Num].Measurable & MEASURE_END) == 0)
	{
		InterlockedOr((volatile LONG*)(&Test_Case[_Test_Case_Num].Measurable), MEASURE_END);
	}
}

void DHDebugger::Key_Input_Thread()
{
	while (!Is_End)
	{
		/// �׽�Ʈ���̽� 1~3��.
		///	1. ��Ʈ �÷����� ù��° �ڸ��� 1�ΰ�츦 üũ => & ����.
		/// 2. & ������ ����� 1���� ũ�ٸ�, �ش� �÷��װ� ���� ������ ��Ÿ��.
		/// 3-1. �÷��װ� ���� �ִٸ� ���ְ�
		/// 3-2. �÷��װ� ���� �ִٸ� ���ش�.
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

		/// �α� ����� ����.
		if (m_KeyIO->Key_Down(VK_F4))
		{
			/// ���ʿ��� ������ ������ �����ϱ� ���� ó�� �����ڿ��� ���� �ΰŸ� ������ �ʴ´�.
			if (m_LogManager == nullptr)
			{
				m_LogManager = new DHLogger;
			}
			/// �ΰ� ����.
			else
			{
				m_LogManager->Release();
				m_LogManager = nullptr;
			}
		}

		/// �׽�Ʈ ���̽� 1~N���� ���� ����.
		if (m_KeyIO->Key_Down(VK_F5))
		{
			// ��ü ���� ����
			if (Is_Total_Measure)
			{
				system("cls");
				// ��� �׽�Ʈ ���̽��� ������ �Ѵٴ� �÷��� ����.
				for (int i = 0; i < MAX_CASE_COUNT; i++)
				{
					Measurable_Test_Case_Print_KeyBoard((Test_Case_Number)i);
					//InterlockedOr((volatile LONG*)(&Test_Case[i].Measurable), MEASURABLE);
					//Test_Case[i].Measurable |= MEASURABLE;
				}

				InterlockedExchange((volatile LONG*)&Is_Total_Measure, !Is_Total_Measure);
				//Is_Total_Measure = !Is_Total_Measure;
			}
			// ��ü ���� ����
			else
			{
				// ��� �׽�Ʈ ���̽��� �������� �ʴ´ٴ� �÷��� ����.
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

		// ������ �ð��� �������� �α׸� ����Ѵ�.
		if ((Pass_Time.count() * 1000) > Current_Set_TimerSec)
		{
			Pass_Time = std::chrono::milliseconds::zero();

			// ��� �׽�Ʈ ���̽��� ���Ͽ�
			for (int i = 0; i < MAX_CASE_COUNT; i++)
			{
				// �ش絥���͸� �����ش�.
				Measurable_Test_Case_Print_Timer((Test_Case_Number)i);
			}
		}
	}
}

void DHDebugger::Initialize_KeyBoard_Mode()
{
	m_KeyIO = new DHKeyIO;
	/// Ű���带 ���� ����� ��忡�� ���Ǵ� ������.
	Use_Mode_Thread = new std::thread(std::bind(&DHDebugger::Key_Input_Thread, this));
}

void DHDebugger::Initialize_Timer_Mode()
{
	/// Ÿ�̸Ӹ� ���� ����� ��忡�� ���Ǵ� ������.
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
	// Ÿ�̸� ���� (���ʸ��� �α׸� ������?)
	if (_Timer_Sec < 0)
	{
		printf_s("[DHDebugger Ÿ�̸� ���ÿ���] %f �ʷ� �ùٸ������� ���ڷ� �����Ǿ����ϴ�. �⺻���� 5�ʷ� �ٲ�ϴ�. \n", _Timer_Sec);
		Current_Set_TimerSec = 5000.f;
	}
	else
	{
		Current_Set_TimerSec = _Timer_Sec * 1000;
	}
}
