#pragma once

#define MAX_WORD 1024
#define LOG_DIRECTORY _T("..\\Log")
#define LOG_PATH _T("..\\Log\\")

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
	// 외부에서 로거의 동작을 지시한 경우에만 로거가 동작.
	bool Is_End_Loop = false;
	// 로그가 발생한 시간을 저장하기 위함.
	SYSTEMTIME Local_Time;
	// Read/Write HANDLE
	HANDLE hFileWirte = nullptr;
	std::thread* WorkThread = nullptr;
	// 로그 메세지를 받아서 처리할 로그큐.
	Concurrency::concurrent_queue<std::pair<int,TCHAR*>> Log_Queue;
	// 로그 파일을 저장할 경로
	TCHAR* File_Name = nullptr;

public:
	DHLOG_DLL DHLogger();
	~DHLogger();
	// 로컬시간 불러오기
	DHLOG_DLL SYSTEMTIME _GetLocalTime();
	// 파일 쓰기
	DHLOG_DLL void WriteLog(LogType _Log_Type, TCHAR* _Write_Data);
	// 로거 해제.
	DHLOG_DLL void Release();
	// 쓰레드 함수.
	void WorkThreadFunction();

private:
	// Log 타입을 string으로 받아옴.
	TCHAR* GetLogTypeText(LogType _Log_Type);
	// 현재 시간로그 받아오기.
	TCHAR* GetCurrentTimeText();
	// 현재 시간 파일 제목 받아오기.
	TCHAR* GetCurrentTimeTitle();
};

