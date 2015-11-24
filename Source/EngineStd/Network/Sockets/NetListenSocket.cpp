#include "EngineStd.h"
#include "NetListenSocket.h"

NetListenSocket::NetListenSocket(Context* context) : NetSocket(context)
{
	m_PortNum = 0;
}

NetListenSocket::NetListenSocket(Context* context, unsigned short portNum) : NetSocket(context_, String("localhost"))
{
	m_PortNum = portNum;
	Init(portNum);
}

void NetListenSocket::Init(unsigned short portNum)
{
	if (m_pNetwork->StartServer(portNum))
	{
		m_PortNum = portNum;
		URHO3D_LOGDEBUG(String("Server was started"));
	}
	else
	{
		URHO3D_LOGDEBUG(String("Server starting was failed. Check port number " + String(m_PortNum) + " ?"));
	}

	m_pConnection = m_pNetwork->GetServerConnection();
}

void NetListenSocket::Shutdown()
{
	m_pNetwork->StopServer();
}

NetListenSocket::~NetListenSocket()
{
	
}




// ----------------------- DELEGATES ----------------------
void NetListenSocket::VInitializeDelegates()
{
	

}

void NetListenSocket::VDestroyAllDelegates()
{
	

}