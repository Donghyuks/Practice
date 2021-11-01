
#include "LogManager.h"

LogManager::LogManager(CString _Path)
	:Log_Loof(true)
{
	// 크리티컬 섹션을 초기화합니다.
	InitializeCriticalSection(&CS);
	Path = _Path;
	File_Name = GetCurrentTimeTitle();
	SetCurrentDirectory(String_to_TCHAR(Path));
	hFileWirte = CreateFile(String_to_TCHAR(File_Name), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileWirte == INVALID_HANDLE_VALUE)
	{
		const DWORD error = GetLastError();

		_tprintf(_T("File Write Error!\n"));
	}

	CreateLogDir(NULL);

	unsigned int uiThreadID = 0;
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, ThreadF, this, CREATE_SUSPENDED, &uiThreadID));
	ResumeThread(hThread);
}

LogManager::~LogManager()
{
	/// 쓰레드 종료.
	LogEnd();
	WaitForSingleObject(hThread, INFINITE);

	/// 큐 해제 및 초기화. (빈 큐와 바꿔치기 하면 queue 내의 메모리를 해제함과 동시에 queue를 초기화 할 수 있다.)
	std::queue<CString> empty_queue;
	std::swap(Log_Queue, empty_queue);

	CloseHandle(hFileWirte);
}

CString LogManager::GetCurrentTimeLog()
{
	TCHAR Write_Current_Time[MAX_WRITE_WORD];
	GetLocalTime(&Local_Time);
	_stprintf_s(Write_Current_Time, _T("[Log] %02d-%02d-%02d [%02d:%02d:%02d] "),
		Local_Time.wYear, Local_Time.wMonth, Local_Time.wDay,
		Local_Time.wHour, Local_Time.wMinute, Local_Time.wSecond);

	return TCHAR_to_String(Write_Current_Time);
}

CString LogManager::GetCurrentTimeTitle()
{
	TCHAR Write_Current_Time[MAX_WRITE_WORD];
	GetLocalTime(&Local_Time);
	_stprintf_s(Write_Current_Time, _T("[thLog] %02d-%02d-%02d %02dh %02dm %02ds.log"),
		Local_Time.wYear, Local_Time.wMonth, Local_Time.wDay,
		Local_Time.wHour, Local_Time.wMinute, Local_Time.wSecond);

	return TCHAR_to_String(Write_Current_Time);
}

CString LogManager::GetLogType(eLogType _Log_Type)
{
	switch (_Log_Type)
	{
	case eLogType::CollisionState:
		return CString("[충돌상태] ");
		break;
	case eLogType::IDleState:
		return CString("[대기상태] ");
		break;
	default:
		return CString("[정의되지 않은 상태입니다.] ");
		break;
	}
}

CString LogManager::TCHAR_to_String(const TCHAR* ptsz)
{
	CString strString;
	strString = (LPCTSTR)ptsz;
	return strString;
}

TCHAR* LogManager::String_to_TCHAR(CString& s)
{
	TCHAR* tchr = (TCHAR*)(LPCTSTR)s;
	return tchr;
}

SYSTEMTIME LogManager::_GetLocalTime()
{
	return Local_Time;
}

bool LogManager::CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	return CreateDirectory(String_to_TCHAR(Path), lpSecurityAttributes);
}

void LogManager::WriteLog(CString _write_data)
{
	EnterCriticalSection(&CS);
	Log_Queue.push(_write_data);
	LeaveCriticalSection(&CS);
}

void LogManager::SetWriteable()
{
	Is_Writeable = true;
}

unsigned int __stdcall LogManager::ThreadF(void* arg)
{
	LogManager* LM = static_cast<LogManager*>(arg);
	return LM->LogStart();
}

unsigned int LogManager::LogStart()
{
	CString temp_str = "";
	DWORD numOfByteWritten = 0;
	WORD wd = 0xFEFF;
	WriteFile(hFileWirte, &wd, 2, &numOfByteWritten, NULL);

	while (Log_Loof)
	{
		if (!Log_Queue.empty())
		{
			EnterCriticalSection(&CS);
			temp_str = Log_Queue.front();
			LeaveCriticalSection(&CS);
			Log_Queue.pop();
			WriteFile(hFileWirte, temp_str, temp_str.GetLength() * sizeof(TCHAR), &numOfByteWritten, NULL);
		}
	}

	DeleteCriticalSection(&CS);

	return 0;
}

void LogManager::LogEnd()
{
	Log_Loof = false;
}
