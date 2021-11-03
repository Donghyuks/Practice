#pragma once

/*
2021.06.25 CDH

	�����ϰ���� ����� ����
	<F1~F3 Ű>
		1. F1~F3 Ű�� ���� �׽�Ʈ ���̽� �� 1~3���� ���� �׽�Ʈ ����.
		2. F1~F3 Ű�� �ѹ� �� ������ �׽�Ʈ ����.
		3. ���� ���� �� ������ �����͸� ���.
	<F4 Ű>
		1. F4 Ű�� ���� �α׸� ����� ����.
		2. F4 Ű�� �ѹ� �� ������ �α� ����� ����.
		3. ���� �� �α׸� ����� �����嵵 ���� ����.
	<F5 Ű>
		1. F5 Ű�� ���� �׽�Ʈ ���̽� 1~N ���� ���� ����.
		2. 5�� / 15�� / 30�� �������� �� ���̽��� ���� ������ ����.
		3. ���� ����.
		4. ���� ���� �� ������ �����͸� ���.

2021.06.29 CDH

	����� ����.
		1.	Start_Mark_Timer �Լ��� �ش� �׽�Ʈ ���̽��� ���Ͽ� ȣ���� �Ǿ� �־�� �Ѵ�.
		2.	End_Mark_Timer �Լ��� �ش� ���̽��� ���Ͽ� ȣ���� �Ǿ� �־�� �Ѵ�.
		3.	<F1~F5>�� ������ ���ϴ� ���̽��� ���� ������ ���� / ���� �Ѵ�.

2021.11.02 CDH

	DH����� DLL �������� ����.

2021.11.03 CDH
	
	��������
		1.	������ Key ���� ���� �Է¹���� ȿ���� ������? => �ǿ뼺�� �������� �Ǵ�..
			����ڰ� ���ϸ� �۵��ϴ� ���� ����! (Default�� Ű���带 ���� �Է����� ���� �ƴ�)
		2.	Default ���·� Ư�� �ð��� �����Ͽ� �۵��ϴ� ����ŷ� ����.
*/

/// ������ ���¸� ��Ÿ���� ����.
#define MEASURABLE		0b100	// ������ �� ���ΰ��� ���� ����.
#define	MEASURE_START	0b010	// ������ �����ϴ� �κ��� �����ϴ���?
#define MEASURE_END		0b001	// ������ �����ϴ� �κ��� �����ϴ���?
#define MEASURABLE_STAT	0b111	// ��� ����� ������ ���. (���� ���� ����)'
#define MEASURE_DEFAULT	0b000	// ���� �ʱ�ȭ. (��� ������ 0���� ����)

#ifdef DHDEBBUGER_EXPORTS
#define DHDEBUG_DLL __declspec(dllexport)
#else
#define DHDEBUG_DLL __declspec(dllimport)
#endif

/// �ð� ���������� C++11 ������ chrono�� �Ẹ��.
#include <tchar.h>
#include <chrono>

namespace std
{
	class thread;
}

class DHLogger;
class DHKeyIO;

/// ����� �Ŵ���.
class DHDebugger
{
public:

	enum class DHDEBUG_DLL DHDebugger_Mode
	{
		/// F1 ~ F5 Ű�� ���� �۵��ϴ� ���.
		KeyBoard_Mode,
		/// Ư�� �ð��� �����Ͽ� �ð����� ����ϴ� ���.
		Timer_Mode
	};

	enum DHDEBUG_DLL Test_Case_Number
	{
		CASE_01, CASE_02,
		CASE_03, CASE_04,
		CASE_05, CASE_06,
		CASE_07, CASE_08,
		CASE_09, CASE_10,
		MAX_CASE_COUNT			// �� �׽�Ʈ ���̽� ����.
	};

private:
	DHLogger* m_LogManager = nullptr;
	DHKeyIO* m_KeyIO = nullptr;

	/// ������ Ƚ���� �ð��� ���踦 �����ص�. *map�� ����ϸ� �ƹ����� ������.. => ���ο� ����ü �����.
	struct Test_Data
	{
		unsigned int	Measurable = MEASURE_DEFAULT;	// ���� ������ ���������� ��Ÿ��.
		unsigned int	Test_Count = 0;					// �׽�Ʈ ���� Ƚ��.
		std::chrono::system_clock::time_point Start_Time;
		std::chrono::duration<double>Total_Time = std::chrono::milliseconds::zero();	// ��� �׽�Ʈ �����ð��� ��.
	};

	/// Timer ���� ����� ������ �ð� ������.
	// ���� ���� �ð�
	std::chrono::system_clock::time_point Check_Start_Time;
	// ���� �ð�
	std::chrono::system_clock::time_point Current_Time;
	// ������ �ð�
	std::chrono::duration<double>Pass_Time = std::chrono::milliseconds::zero();

	/// ���� ����� ����
	DHDebugger_Mode Current_Mode;
	/// ���� ���õǾ��ִ� Ÿ�̸� �ð�(�и�)��.
	float Current_Set_TimerSec = 5000.f;
	/// �׽�Ʈ ���̽��� �ִ� 10������ ���� ����.
	Test_Data Test_Case[MAX_CASE_COUNT];
	/// ���Ḧ �˸��� �÷���.
	bool Is_End = false;
	/// ��ü �������� �� ����
	bool Is_Total_Measure = false;
	/// Ű �Է��� ���� ������.
	std::thread* Use_Mode_Thread = nullptr;
	/// Log �� Text�� ������ ����.
	bool Use_Logger = true;

private:
	// �ش� �׽�Ʈ ���̽� �ʱ�ȭ.
	void Clear_Test_Case(Test_Case_Number _Test_Case_Num);
	// �ش� �׽�Ʈ ���̽��� ���� ���� ���� ���� ���� ������ ���� ��쿡 ���� ó��.
	void Measurable_Test_Case_Print_KeyBoard(Test_Case_Number _Test_Case_Num);
	// Ÿ�̸� ���� �����ϴ� Print
	void Measurable_Test_Case_Print_Timer(Test_Case_Number _Test_Case_Num);
	//void Measurable_Test_Case_Print_KeyBoard(Test_Case_Number _Test_Case_Num);
	// Ű �Է¿� ���� ������.
	void Key_Input_Thread();
	// Ÿ�̸ӿ� ���� ������.
	void Timer_Thread();
	// Ű���� ����� �� �ʱ�ȭ.
	void Initialize_KeyBoard_Mode();
	// Ÿ�̸� ����� �� �ʱ�ȭ.
	void Initialize_Timer_Mode();

public:
	/// <summary>
	/// ����� ����
	/// </summary>
	/// <param name="_Debugger_Mode"> ����� ���� Ű���带���� ��� / �ð������� ���� ��尡 �ִ�. </param>
	/// <param name="_Use_Logger"> Text���Ϸ� ����� ���� ���� </param>
	/// <param name="_Title_Name"> Text���Ϸ� ����Ѵٸ� �ش� ������ Title �̸� ����. </param>
	/// <returns></returns>
	DHDEBUG_DLL  DHDebugger(DHDebugger_Mode _Debugger_Mode, bool _Use_Logger = true, const TCHAR* _Title_Name = _T("DHDEBUG"));
	DHDEBUG_DLL  ~DHDebugger();

	/// �⺻������ �������� ������ ���ܾ� �ð��� ������ ������ �� �ִ�. (F1~3, F5 ��� �̿��)
	// ���� ��ġ�� �ش� Test_Case�� ���� �ð����� ������ �����. 
	DHDEBUG_DLL void Start_Mark_Timer(Test_Case_Number _Test_Case_Num);
	// Test_Case �� �ð����� ��������.
	DHDEBUG_DLL void End_Mark_Timer(Test_Case_Number _Test_Case_Num);
	// ���� ���� ����� ���� Ÿ���� �ٿ��� �α����� ���. ex) [�ſ�ſ��߿�] LOG...
	DHDEBUG_DLL void WriteLog(const TCHAR* _Log_Tag, const TCHAR* _Write_Data, ...);
	// ���� Ÿ�̸� ��带 ���� ����Ÿ� ���������� Ÿ�̸Ӹ� ���ʷ� ��������? (�������ϸ� �⺻ 5��)
	DHDEBUG_DLL void SetTimerSec(float _Timer_Sec);

};

