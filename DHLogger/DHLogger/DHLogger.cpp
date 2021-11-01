#include "pch.h"
#include "DHLogger.h"

DHLogger::DHLogger(const TCHAR* _Path /*= _T("..\\Log\\")*/)
{
	File_Path = const_cast<TCHAR*>(_Path);
	
}

DHLogger::~DHLogger()
{

}

TCHAR* DHLogger::GetCurrentTimeLog()
{

}

TCHAR* DHLogger::GetCurrentTimeTitle()
{

}

TCHAR* DHLogger::GetLogType(LogType _Log_Type)
{

}

SYSTEMTIME DHLogger::_GetLocalTime()
{

}

bool DHLogger::CreateLogDir(LPSECURITY_ATTRIBUTES lpSecurityAttributes /*= NULL*/)
{

}

void DHLogger::WriteLog(TCHAR* _write_data)
{

}

void DHLogger::SetWriteable()
{

}

void DHLogger::WorkThreadFunction()
{

}
