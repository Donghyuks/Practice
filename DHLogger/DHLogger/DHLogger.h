#pragma once

#define MAX_WORD 1024

#ifdef DHLOGGER_EXPORTS
#define DHLOG_DLL __declspec(dllexport)
#else
#define DHLOG_DLL __declspec(dllimport)
#endif

#include <tchar.h>
#include <concurrent_queue.h>

namespace std
{
	class thread;
}
struct SYSTEMTIME;

enum class LogType
{
	WARNING_Log,
	ASSERT_Log,
	COMPLETE_Log
};

class DHLogger
{
private:
	// �ܺο��� �ΰ��� ������ ������ ��쿡�� �ΰŰ� ����.
	bool Is_Writeable = false;
	// �αװ� �߻��� �ð��� �����ϱ� ����.
	SYSTEMTIME* Local_Time = nullptr;

	// Read/Write HANDLE
	HANDLE hFileWirte = nullptr;
	std::thread* WorkThread = nullptr;

	HANDLE hThread = nullptr;

	Concurrency::concurrent_queue<TCHAR*> Log_Queue;

	// �α� ������ ������ ���
	TCHAR* File_Path;
	TCHAR* File_Name;

	// ���� �ð��� ����
	TCHAR* Current_Time_Log;

public:
	DHLogger(const TCHAR* _Path = _T("..\\Log\\"));
	~DHLogger();

	// ���� �ð��α� �޾ƿ���.
	TCHAR* GetCurrentTimeLog();
	// ���� �ð� ���� ���� �޾ƿ���.
	TCHAR* GetCurrentTimeTitle();
	// Log Ÿ���� string���� �޾ƿ�.
	TCHAR* GetLogType(LogType _Log_Type);
	// ���ýð� �ҷ�����
	SYSTEMTIME _GetLocalTime();
	// Dir ����
	bool CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	// ���� ����
	void WriteLog(TCHAR* _write_data);
	// WriteLog �� ȣ���� �κп��� ���������� ����ϱ� ���ؼ��� Writeable ���·� �ٲ��־�� �Ѵ�.
	void SetWriteable();

private:
	// ������ �Լ�.
	void WorkThreadFunction();

};

