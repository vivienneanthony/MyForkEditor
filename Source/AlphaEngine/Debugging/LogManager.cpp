#include "AlphaEngineStd.h"
#include "LogManager.h"


//Singleton
class LogManger;
LogManager* g_pLogManager = nullptr;

LogManager::LogManager()
{
	m_pLog = nullptr;
}

LogManager::~LogManager()
{
	m_pLog->Close();
	delete m_pLog;
}

void LogManager::Init(const char* logFileName)
{
	m_pLog = new Log(g_pApp->GetContext());
	m_pLog->Open(logFileName);
	m_pLog->SetQuiet(false);
}

