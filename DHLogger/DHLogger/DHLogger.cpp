#include "pch.h"
#include "DHLogger.h"
#pragma warning(disable:4996)

DHLogger::DHLogger()
{
	// ������ �̸�.
	File_Title = const_cast<TCHAR*>(DEFAULT_FILE_NAME);
	// ���� Log��� ������ ������ �Ǿ��ִ°�? �� Ȯ���� �� �����Ǿ� ���� �ʴٸ� ������ �����Ѵ�.
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(LOG_DIRECTORY))
	{
		const DWORD error = GetLastError();
		CreateDirectory(LOG_DIRECTORY, NULL);
	}

	// ������ Log ������ txt ������ ����� ���� ��θ� �����ϰ� �ڵ��� �޾ƿ´�.
	SetCurrentDirectory(LOG_PATH);
	hFileWirte = CreateFile(GetCurrentTimeTitle(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileWirte == INVALID_HANDLE_VALUE)
	{
		const DWORD error = GetLastError();

		_tprintf(_T("[DHLogger ������ ����] hFileWrite�� �ùٸ��� �������� ���߽��ϴ�.\n"));
	}

	//CreateDirectory(String_to_TCHAR(Path), lpSecurityAttributes);

	WorkThread = new std::thread(std::bind(&DHLogger::WorkThreadFunction, this));
}

DHLOG_DLL DHLogger::DHLogger(const TCHAR* _File_Title)
{
	// ������ �̸�.
	File_Title = const_cast<TCHAR*>(_File_Title);
	// ���� Log��� ������ ������ �Ǿ��ִ°�? �� Ȯ���� �� �����Ǿ� ���� �ʴٸ� ������ �����Ѵ�.
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(LOG_DIRECTORY))
	{
		const DWORD error = GetLastError();
		CreateDirectory(LOG_DIRECTORY, NULL);
	}

	// ������ Log ������ txt ������ ����� ���� ��θ� �����ϰ� �ڵ��� �޾ƿ´�.
	SetCurrentDirectory(LOG_PATH);
	hFileWirte = CreateFile(GetCurrentTimeTitle(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileWirte == INVALID_HANDLE_VALUE)
	{
		const DWORD error = GetLastError();

		_tprintf(_T("[DHLogger ������ ����] hFileWrite�� �ùٸ��� �������� ���߽��ϴ�.\n"));
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
		return const_cast<TCHAR*>(_T("[���]"));
		break;
	case LogType::ERROR_Log:
		return const_cast<TCHAR*>(_T("[����]"));
		break;
	case LogType::COMPLETE_Log:
		return const_cast<TCHAR*>(_T("[����]"));
		break;
	default:
		return const_cast<TCHAR*>(_T("[���ǵ��� ����Ÿ��]"));
		break;
	}
}

SYSTEMTIME DHLogger::_GetLocalTime()
{
	return Local_Time;
}

void DHLogger::WriteLog(LogType _Log_Type, const TCHAR* _Write_Data)
{
	// �α� Ÿ�Կ� ���� �ؽ�Ʈ �޾ƿ���.
	std::wstring _LogType_Text(GetLogTypeText(_Log_Type));
	// ���� �ð� �ؽ�Ʈ
	std::wstring _Time_Text(GetCurrentTimeText());
	// "[Ÿ��][�ð�] �޼���" �������� �����.
	std::wstring _Total_Text = _LogType_Text + _Time_Text + _Write_Data +_T("\r\n");

	// �ش� ���� �����ϰ� ���� �־���.
	TCHAR* _T_Total_Text = new TCHAR[_Total_Text.size()+1];
	wcscpy(_T_Total_Text, (wchar_t*)(_Total_Text.c_str()));
	Log_Queue.push({ _Total_Text.size(), _T_Total_Text });
}

DHLOG_DLL void DHLogger::WriteLog(const TCHAR* _Log_Title, const TCHAR* _Write_Data)
{
	// ���� �ð� �ؽ�Ʈ
	std::wstring _Time_Text(GetCurrentTimeText());
	// "[Ÿ��][�ð�] �޼���" �������� �����.
	std::wstring _Total_Text = _T("[") + std::wstring(_Log_Title) + _T("]") + _Time_Text + _Write_Data + _T("\r\n");

	// �ش� ���� �����ϰ� ���� �־���.
	TCHAR* _T_Total_Text = new TCHAR[_Total_Text.size() + 1];
	wcscpy(_T_Total_Text, (wchar_t*)(_Total_Text.c_str()));
	Log_Queue.push({ _Total_Text.size(), _T_Total_Text });
}

void DHLogger::Release()
{
	Is_End_Loop = true;
	// WorkThread�� ���Ḧ ��ٸ�.
	WorkThread->join();
	// ����ߴ� Queue �� �ڿ� ����.
	Log_Queue.~concurrent_queue();
	// ����ߴ� File Handle ����.
	CloseHandle(hFileWirte);
}

void DHLogger::WorkThreadFunction()
{
	std::pair<int, TCHAR*> Get_Log_Message;
	DWORD numOfByteWritten = 0;
	WORD wd = 0xFEFF;
	WriteFile(hFileWirte, &wd, 2, &numOfByteWritten, NULL);

	// �ۿ��� ����ȣ���� �ö����� ����ؼ� ����Ѵ�.
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

	// ���� �����带 �����ߴµ� ��ϵ��� ���� �αװ� �����Ѵٸ� ��� �ۼ��ϰ� ������.
	while (!Log_Queue.empty())
	{
		Log_Queue.try_pop(Get_Log_Message);
		WriteFile(hFileWirte, Get_Log_Message.second, Get_Log_Message.first * 2, &numOfByteWritten, NULL);
		delete[] (Get_Log_Message.second);
	}
}
