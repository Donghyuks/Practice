#include "DHLogger.h"

int main()
{
	// �ΰ� ����
	DHLogger* m_Logger = new DHLogger;

	m_Logger->WriteLog(LogType::COMPLETE_Log, const_cast<TCHAR*>(_T("�̰� �Ϻ��ؿ�!")));
	m_Logger->WriteLog(LogType::ERROR_Log, const_cast<TCHAR*>(_T("�̰� ��������!")));
	m_Logger->WriteLog(LogType::WARNING_Log, const_cast<TCHAR*>(_T("�̰� �����ؿ�!")));

	m_Logger->Release();
}