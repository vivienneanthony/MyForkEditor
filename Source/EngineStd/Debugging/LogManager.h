#ifndef LOGMANAGER_H
#define LOGMANAGER_H

class LogManager
{
public:
	LogManager();
	~LogManager();

	void Init(const char* logFileName);

private:
	Log* m_pLog;
};

extern LogManager* g_pLogManager;

#endif
