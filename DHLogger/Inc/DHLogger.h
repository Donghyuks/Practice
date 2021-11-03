#pragma once

#define MAX_WORD 1024
#define LOG_DIRECTORY _T("..\\Log")
#define LOG_PATH _T("..\\Log\\")
#define DEFAULT_FILE_NAME _T("STATE LOG")

#ifdef DHLOGGER_EXPORTS
#define DHLOG_DLL __declspec(dllexport)
#else
#define DHLOG_DLL __declspec(dllimport)
#endif

#include <tchar.h>
#include <windows.h>
#include <concurrent_queue.h>

namespace std
{
	class thread;
}

enum class DHLOG_DLL LogType
{
	WARNING_Log,
	ERROR_Log,
	COMPLETE_Log
};

class DHLogger
{
private:
	// �ܺο��� �ΰ��� ������ ������ ��쿡�� �ΰŰ� ����.
	bool Is_End_Loop = false;
	// �αװ� �߻��� �ð��� �����ϱ� ����.
	SYSTEMTIME Local_Time;
	// Read/Write HANDLE
	HANDLE hFileWirte = nullptr;
	std::thread* WorkThread = nullptr;
	// �α� �޼����� �޾Ƽ� ó���� �α�ť.
	Concurrency::concurrent_queue<std::pair<int,TCHAR*>> Log_Queue;
	// �α� ������ ������ ���
	TCHAR* File_Name = nullptr;
	// ������ ����
	TCHAR* File_Title = nullptr;

public:
	// �⺻ ������ ȣ��� ������ �̸��� STATE LOG ��� �ٴ´�.
	DHLOG_DLL DHLogger();
	// ������ �̸��� ����.
	DHLOG_DLL DHLogger(const TCHAR* _File_Title);
	~DHLogger();
	// ���ýð� �ҷ�����
	DHLOG_DLL SYSTEMTIME _GetLocalTime();
	// ���� ����
	DHLOG_DLL void WriteLog(LogType _Log_Type, const TCHAR* _Write_Data);
	// ������ LogType �� �����ϰ� ����� ���� Ÿ���� �ٿ��� �α����� ���. ex) [�ſ�ſ��߿�] LOG...
	DHLOG_DLL void WriteLog(const TCHAR* _Log_Title, const TCHAR* _Write_Data);
	// �ΰ� ����.
	DHLOG_DLL void Release();
	// ������ �Լ�.
	void WorkThreadFunction();

private:
	// Log Ÿ���� string���� �޾ƿ�.
	TCHAR* GetLogTypeText(LogType _Log_Type);
	// ���� �ð��α� �޾ƿ���.
	TCHAR* GetCurrentTimeText();
	// ���� �ð� ���� ���� �޾ƿ���.
	TCHAR* GetCurrentTimeTitle();
};

