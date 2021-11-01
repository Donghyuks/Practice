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

	// 외부에서 로거의 동작을 지시한 경우에만 로거가 동작.
	bool Is_Writeable = false;
	// 로거의 쓰레드 동작 플래그.
	bool Log_Loof = false;
	// 로그가 발생한 시간을 저장하기 위함.
	SYSTEMTIME Local_Time;
	HANDLE hFileWirte = nullptr;
	HANDLE hThread = nullptr;
	
	CRITICAL_SECTION CS;	// 크리티컬 섹션

	std::queue<CString> Log_Queue;

	// 로그파일을 저장할 경로.
	CString Path;
	CString File_Name;

	// 현재 시간 저장.
	CString Current_Time_Log;
public:
	LogManager(CString _Path = "..\\Log\\");
	~LogManager();

	// 현재 시간로그 받아오기.
	CString GetCurrentTimeLog();
	// 현재 시간 파일 제목 받아오기.
	CString GetCurrentTimeTitle();
	// Log 타입을 string으로 받아옴.
	CString GetLogType(eLogType _Log_Type);
	// TCHAR 을 String으로 변환하여 리턴.
	CString TCHAR_to_String(const TCHAR* ptsz);
	// String 을 TCHAR으로 변환하여 리턴.
	TCHAR* String_to_TCHAR(CString& s);
	// 로컬시간 불러오기
	SYSTEMTIME _GetLocalTime();
	// Dir 생성
	bool CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	// 파일 쓰기
	void WriteLog(CString _write_data);
	// WriteLog 를 호출한 부분에서 실질적으로 기록하기 위해서는 Writeable 상태로 바꿔주어야 한다.
	void SetWriteable();

	static unsigned int __stdcall ThreadF(void* arg);
	unsigned int LogStart();
	void LogEnd();
};

