#include "AlphaEngineStd.h"
#include "ClientSocketManager.h"
#include "Network/Sockets/RemoteEventSocket.h"
#include "GameLogic/BaseGameLogic.h"

ClientSocketManager::ClientSocketManager(Context* context, String hostName, unsigned short port, const VariantMap& identify) : BaseSocketManager(context)
{
	g_pSocketManager = this;
	m_Port = port;
	m_HostName = hostName;
	m_ClientIdentify = identify;
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

	RemoteEventSocket* pSocket = new RemoteEventSocket(context_);

	if (!pSocket->Connect(m_HostName, m_Port, g_pApp->GetGameLogic()->GetScene(), m_ClientIdentify))
	{
		SAFE_DELETE(pSocket);
		return false;
	}

	AddSocket(pSocket);

	return true;
}