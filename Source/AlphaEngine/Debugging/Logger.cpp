#include "AlphaEngineStd.h"
#include "Logger.h"
#include "LogManager.h"


void Logger::Init(const char* logFileName)
{
	if (g_pLogManager == nullptr)
	{
		g_pLogManager = new LogManager();
		g_pLogManager->Init(logFileName);
	}
	}

void Logger::Shutdown()
{
	if (g_pLogManager)
	{
		delete g_pLogManager;
	}
}



