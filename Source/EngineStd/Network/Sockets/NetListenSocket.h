#pragma once

#include "NetSocket.h"

// Class for listening client connections, accepting them, adding new sockets in the global socket manager.

class NetListenSocket : public NetSocket
{
	URHO3D_OBJECT(NetListenSocket, NetSocket)

public:
	NetListenSocket(Context* context);
	NetListenSocket(Context* context, unsigned short portNum);
	virtual ~NetListenSocket();

	void Init(unsigned short portNum);
	void Shutdown();
	

protected:
	virtual void VInitializeAllDelegates();	// Subsribe to events to this net socket.
	virtual void VDestroyAllDelegates();	// Unsubscribe from all events to this net socket

protected:
	unsigned short m_PortNum;
};