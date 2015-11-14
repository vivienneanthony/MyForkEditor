#include "EngineStd.h"
#include "ClientSocketManager.h"

ClientSocketManager::ClientSocketManager(Context* context, String hostName, unsigned short port) : BaseSocketManager(context)
{
	g_pSocketManager = this;
	m_Port = port;
	m_HostName = hostName;
}

ClientSocketManager::~ClientSocketManager()
{
	
}

bool ClientSocketManager::Connect()
{
	if(!BaseSocketManager::Init())
	{
		return false;	
	}


	return true;
}