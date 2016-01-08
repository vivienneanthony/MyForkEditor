#include "AlphaEngineStd.h"
#include "GameLogic/BaseGameLogic.h"
#include "NetListenSocket.h"

NetListenSocket::NetListenSocket(Context* context) : NetSocket(context)
{
	m_PortNum = 0;
	VInitializeAllDelegates();
}

NetListenSocket::NetListenSocket(Context* context, unsigned short portNum) : NetSocket(context, String("localhost"))
{
	m_PortNum = portNum;
	Init(m_PortNum);
	VInitializeAllDelegates();
}

NetListenSocket::~NetListenSocket()
{
	Shutdown();
}


void NetListenSocket::Init(unsigned short portNum)
{
	if (m_pNetwork->StartServer(portNum))
	{
		m_PortNum = portNum;

		if (m_pNetwork->IsServerRunning())
		{
			g_pApp->GetGameLogic()->SetServerCreated(true);
		}
		URHO3D_LOGDEBUG(String("Server was started"));
	}
	else
	{
		g_pApp->GetGameLogic()->SetServerCreated(false);
		URHO3D_LOGDEBUG(String("Server starting was failed. Check port number " + String(m_PortNum) + " ?"));
	}
	m_pConnection = m_pNetwork->GetServerConnection();
}

void NetListenSocket::Shutdown()
{
	m_pNetwork->StopServer();
}





// ----------------------- DELEGATES ----------------------
void NetListenSocket::VInitializeAllDelegates()
{
	

}

void NetListenSocket::VDestroyAllDelegates()
{
	

}