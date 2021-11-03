#include "DHLogger.h"

int main()
{
	// 로거 생성
	//DHLogger* m_Logger = new DHLogger;
	// 로거 생성 2 (지정한 이름으로 생성)
	DHLogger* m_Logger = new DHLogger(_T("DEBUG LOG"));

	m_Logger->WriteLog(LogType::COMPLETE_Log, _T("이건 완벽해요!"));
	m_Logger->WriteLog(LogType::ERROR_Log, _T("이건 에러에요!"));
	m_Logger->WriteLog(LogType::WARNING_Log,_T("이건 위험해요!"));

	m_Logger->WriteLog(_T("사용자지정 상태"),_T("이건 위험해요!"));

	m_Logger->Release();
}