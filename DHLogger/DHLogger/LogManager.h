#pragma once

#ifdef DHLOGGER_EXPORTS
#define DHLOG_DLL __declspec(dllexport)
#else
#define DHLOG_DLL __declspec(dllimport)
#endif

#define MAX_WRITE_WORD 256

enum class eLogType
{
	CollisionState,
	IDleState,
};

class LogManager
{
private:

	// �ܺο��� �ΰ��� ������ ������ ��쿡�� �ΰŰ� ����.
	bool Is_Writeable = false;
	// �ΰ��� ������ ���� �÷���.
	bool Log_Loof = false;
	// �αװ� �߻��� �ð��� �����ϱ� ����.
	SYSTEMTIME Local_Time;
	HANDLE hFileWirte = nullptr;
	HANDLE hThread = nullptr;
	
	CRITICAL_SECTION CS;	// ũ��Ƽ�� ����

	std::queue<CString> Log_Queue;

	// �α������� ������ ���.
	CString Path;
	CString File_Name;

	// ���� �ð� ����.
	CString Current_Time_Log;
public:
	LogManager(CString _Path = "..\\Log\\");
	~LogManager();

	// ���� �ð��α� �޾ƿ���.
	CString GetCurrentTimeLog();
	// ���� �ð� ���� ���� �޾ƿ���.
	CString GetCurrentTimeTitle();
	// Log Ÿ���� string���� �޾ƿ�.
	CString GetLogType(eLogType _Log_Type);
	// TCHAR �� String���� ��ȯ�Ͽ� ����.
	CString TCHAR_to_String(const TCHAR* ptsz);
	// String �� TCHAR���� ��ȯ�Ͽ� ����.
	TCHAR* String_to_TCHAR(CString& s);
	// ���ýð� �ҷ�����
	SYSTEMTIME _GetLocalTime();
	// Dir ����
	bool CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	// ���� ����
	void WriteLog(CString _write_data);
	// WriteLog �� ȣ���� �κп��� ���������� ����ϱ� ���ؼ��� Writeable ���·� �ٲ��־�� �Ѵ�.
	void SetWriteable();

	static unsigned int __stdcall ThreadF(void* arg);
	unsigned int LogStart();
	void LogEnd();
};

