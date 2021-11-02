#include "DHLogger.h"

int main()
{
	// 로거 생성
	DHLogger* m_Logger = new DHLogger;

	m_Logger->WriteLog(LogType::COMPLETE_Log, const_cast<TCHAR*>(_T("이건 완벽해요!")));
	m_Logger->WriteLog(LogType::ERROR_Log, const_cast<TCHAR*>(_T("이건 에러에요!")));
	m_Logger->WriteLog(LogType::WARNING_Log, const_cast<TCHAR*>(_T("이건 위험해요!")));

	m_Logger->Release();
}