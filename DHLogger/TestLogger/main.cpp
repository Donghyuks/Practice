#include "DHLogger.h"

int main()
{
	// �ΰ� ����
	//DHLogger* m_Logger = new DHLogger;
	// �ΰ� ���� 2 (������ �̸����� ����)
	DHLogger* m_Logger = new DHLogger(_T("DEBUG LOG"));

	m_Logger->WriteLog(LogType::COMPLETE_Log, _T("�̰� �Ϻ��ؿ�!"));
	m_Logger->WriteLog(LogType::ERROR_Log, _T("�̰� ��������!"));
	m_Logger->WriteLog(LogType::WARNING_Log,_T("�̰� �����ؿ�!"));

	m_Logger->WriteLog(_T("��������� ����"),_T("�̰� �����ؿ�!"));

	m_Logger->Release();
}