#pragma once

#include "BaseSocketManager.h"

// Extension os BaseScoektManager.
// Manage client side of a game. Attach to a known server.

class ClientSocketManager : public BaseSocketManager
{
	URHO3D_OBJECT(ClientSocketManager, BaseSocketManager)
public:
	ClientSocketManager(Context* context, String hostName, unsigned short port);
	virtual ~ClientSocketManager();

	bool Connect();

protected:
	String m_HostName;
	unsigned short m_Port;
};