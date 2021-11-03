#include "pch.h"
#include "DHLogger.h"
#pragma warning(disable:4996)

DHLogger::DHLogger()
{
	// 파일의 이름.
	File_Title = const_cast<TCHAR*>(DEFAULT_FILE_NAME);
	// 현재 Log라는 폴더가 생성이 되어있는가? 를 확인한 뒤 생성되어 있지 않다면 폴더를 생성한다.
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(LOG_DIRECTORY))
	{
		const DWORD error = GetLastError();
		CreateDirectory(LOG_DIRECTORY, NULL);
	}

	// 생성된 Log 폴더에 txt 파일을 만들기 위해 경로를 설정하고 핸들을 받아온다.
	SetCurrentDirectory(LOG_PATH);
	hFileWirte = CreateFile(GetCurrentTimeTitle(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileWirte == INVALID_HANDLE_VALUE)
	{
		const DWORD error = GetLastError();

		_tprintf(_T("[DHLogger 생성자 오류] hFileWrite를 올바르게 생성하지 못했습니다.\n"));
	}

	//CreateDirectory(String_to_TCHAR(Path), lpSecurityAttributes);

	WorkThread = new std::thread(std::bind(&DHLogger::WorkThreadFunction, this));
}

DHLOG_DLL DHLogger::DHLogger(const TCHAR* _File_Title)
{
	// 파일의 이름.
	File_Title = const_cast<TCHAR*>(_File_Title);
	// 현재 Log라는 폴더가 생성이 되어있는가? 를 확인한 뒤 생성되어 있지 않다면 폴더를 생성한다.
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(LOG_DIRECTORY))
	{
		const DWORD error = GetLastError();
		CreateDirectory(LOG_DIRECTORY, NULL);
	}

	// 생성된 Log 폴더에 txt 파일을 만들기 위해 경로를 설정하고 핸들을 받아온다.
	SetCurrentDirectory(LOG_PATH);
	hFileWirte = CreateFile(GetCurrentTimeTitle(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileWirte == INVALID_HANDLE_VALUE)
	{
		const DWORD error = GetLastError();

		_tprintf(_T("[DHLogger 생성자 오류] hFileWrite를 올바르게 생성하지 못했습니다.\n"));
	}

	//CreateDirectory(String_to_TCHAR(Path), lpSecurityAttributes);

	WorkThread = new std::thread(std::bind(&DHLogger::WorkThreadFunction, this));
}

DHLogger::~DHLogger()
{

}

TCHAR* DHLogger::GetCurrentTimeText()
{
	TCHAR Write_Current_Time[MAX_WORD];
	GetLocalTime(&Local_Time);

	_stprintf_s(Write_Current_Time, _T("[%02d:%02d:%02d] "),
		//Local_Time.wYear, Local_Time.wMonth, Local_Time.wDay,
		Local_Time.wHour, Local_Time.wMinute, Local_Time.wSecond);

	return Write_Current_Time;
}

TCHAR* DHLogger::GetCurrentTimeTitle()
{
	TCHAR Write_Current_Time[MAX_WORD];
	GetLocalTime(&Local_Time);
	_stprintf_s(Write_Current_Time, _T("[%s] %02d-%02d-%02d %02dh %02dm %02ds.log"), File_Title,
		Local_Time.wYear, Local_Time.wMonth, Local_Time.wDay,
		Local_Time.wHour, Local_Time.wMinute, Local_Time.wSecond);

	return Write_Current_Time;
}

TCHAR* DHLogger::GetLogTypeText(LogType _Log_Type)
{
	switch (_Log_Type)
	{
	case LogType::WARNING_Log:
		return const_cast<TCHAR*>(_T("[경고]"));
		break;
	case LogType::ERROR_Log:
		return const_cast<TCHAR*>(_T("[에러]"));
		break;
	case LogType::COMPLETE_Log:
		return const_cast<TCHAR*>(_T("[정상]"));
		break;
	default:
		return const_cast<TCHAR*>(_T("[정의되지 않은타입]"));
		break;
	}
}

SYSTEMTIME DHLogger::_GetLocalTime()
{
	return Local_Time;
}

void DHLogger::WriteLog(LogType _Log_Type, const TCHAR* _Write_Data)
{
	// 로그 타입에 대한 텍스트 받아오기.
	std::wstring _LogType_Text(GetLogTypeText(_Log_Type));
	// 현재 시간 텍스트
	std::wstring _Time_Text(GetCurrentTimeText());
	// "[타입][시간] 메세지" 형식으로 저장됨.
	std::wstring _Total_Text = _LogType_Text + _Time_Text + _Write_Data +_T("\r\n");

	// 해당 값을 생성하고 페어로 넣어줌.
	TCHAR* _T_Total_Text = new TCHAR[_Total_Text.size()+1];
	wcscpy(_T_Total_Text, (wchar_t*)(_Total_Text.c_str()));
	Log_Queue.push({ _Total_Text.size(), _T_Total_Text });
}

DHLOG_DLL void DHLogger::WriteLog(const TCHAR* _Log_Title, const TCHAR* _Write_Data)
{
	// 현재 시간 텍스트
	std::wstring _Time_Text(GetCurrentTimeText());
	// "[타입][시간] 메세지" 형식으로 저장됨.
	std::wstring _Total_Text = _T("[") + std::wstring(_Log_Title) + _T("]") + _Time_Text + _Write_Data + _T("\r\n");

	// 해당 값을 생성하고 페어로 넣어줌.
	TCHAR* _T_Total_Text = new TCHAR[_Total_Text.size() + 1];
	wcscpy(_T_Total_Text, (wchar_t*)(_Total_Text.c_str()));
	Log_Queue.push({ _Total_Text.size(), _T_Total_Text });
}

void DHLogger::Release()
{
	Is_End_Loop = true;
	// WorkThread의 종료를 기다림.
	WorkThread->join();
	// 사용했던 Queue 의 자원 해제.
	Log_Queue.~concurrent_queue();
	// 사용했던 File Handle 해제.
	CloseHandle(hFileWirte);
}

void DHLogger::WorkThreadFunction()
{
	std::pair<int, TCHAR*> Get_Log_Message;
	DWORD numOfByteWritten = 0;
	WORD wd = 0xFEFF;
	WriteFile(hFileWirte, &wd, 2, &numOfByteWritten, NULL);

	// 밖에서 종료호출이 올때까지 계속해서 기록한다.
	while (Is_End_Loop == false)
	{
		if (!Log_Queue.empty())
		{
			Log_Queue.try_pop(Get_Log_Message);
			WriteFile(hFileWirte, Get_Log_Message.second, Get_Log_Message.first * 2, &numOfByteWritten, NULL);
			delete[] (Get_Log_Message.second);
		}

		Sleep(0);
	}

	// 만약 쓰레드를 종료했는데 기록되지 않은 로그가 존재한다면 모두 작성하고 끝낸다.
	while (!Log_Queue.empty())
	{
		Log_Queue.try_pop(Get_Log_Message);
		WriteFile(hFileWirte, Get_Log_Message.second, Get_Log_Message.first * 2, &numOfByteWritten, NULL);
		delete[] (Get_Log_Message.second);
	}
}
