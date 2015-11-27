#pragma once


#include "EngineStd/Network/Managers/ClientSocketManager.h"

// Extension of BaseScoektManager.
// Manage client side of a game. Attach to a known server.

class HanagrsClientSocketManager : public ClientSocketManager
{
	URHO3D_OBJECT(HangarsClientSocketManager, ClientSocketManager)
public:
	HanagrsClientSocketManager(Context* context, String hostName, unsigned short port, VariantMap& identify);
	virtual ~HanagrsClientSocketManager();

	virtual bool Connect();

};