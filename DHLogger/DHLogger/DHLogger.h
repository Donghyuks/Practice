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
	// 외부에서 로거의 동작을 지시한 경우에만 로거가 동작.
	bool Is_Writeable = false;
	// 로그가 발생한 시간을 저장하기 위함.
	SYSTEMTIME* Local_Time = nullptr;

	// Read/Write HANDLE
	HANDLE hFileWirte = nullptr;
	std::thread* WorkThread = nullptr;

	HANDLE hThread = nullptr;

	Concurrency::concurrent_queue<TCHAR*> Log_Queue;

	// 로그 파일을 저장할 경로
	TCHAR* File_Path;
	TCHAR* File_Name;

	// 현재 시간을 저장
	TCHAR* Current_Time_Log;

public:
	DHLogger(const TCHAR* _Path = _T("..\\Log\\"));
	~DHLogger();

	// 현재 시간로그 받아오기.
	TCHAR* GetCurrentTimeLog();
	// 현재 시간 파일 제목 받아오기.
	TCHAR* GetCurrentTimeTitle();
	// Log 타입을 string으로 받아옴.
	TCHAR* GetLogType(LogType _Log_Type);
	// 로컬시간 불러오기
	SYSTEMTIME _GetLocalTime();
	// Dir 생성
	bool CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	// 파일 쓰기
	void WriteLog(TCHAR* _write_data);
	// WriteLog 를 호출한 부분에서 실질적으로 기록하기 위해서는 Writeable 상태로 바꿔주어야 한다.
	void SetWriteable();

private:
	// 쓰레드 함수.
	void WorkThreadFunction();

};

