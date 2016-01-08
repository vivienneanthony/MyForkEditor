#pragma once

#include "BaseSocketManager.h"

// Extension of BaseScoektManager.
// Manage client side of a game. Attach to a known server.

class ClientSocketManager : public BaseSocketManager
{
	URHO3D_OBJECT(ClientSocketManager, BaseSocketManager)
public:
	ClientSocketManager(Context* context, String hostName, unsigned short port, const VariantMap& identify = Variant::emptyVariantMap);
	virtual ~ClientSocketManager();

	virtual bool Connect();

protected:
	String m_HostName;
	unsigned short m_Port;
	VariantMap m_ClientIdentify;
};