#pragma once

#include "NetListenSocket.h"

// ServerSocketManager.
// Alghoritm of ServerSocketManager:
// 1)Initialize server socket manager and attach a listen socket.
// 2)Call DoSelect() on the server socket manager.
// 3)It there is input on the listen socket,  create new socket and attach it to the socket manager.
// 4)Handle input/output socket.

class GameServerListenSocket : public NetListenSocket
{
	URHO3D_OBJECT(GameServerListenSocket, NetListenSocket)

public:
	GameServerListenSocket(Context* context, int portName);
	virtual ~GameServerListenSocket() { }

	virtual void VHandleInput() { }

protected:
	virtual void VInitializeDelegates();	// Subsribe to events to this net socket.
	virtual void VDestroyAllDelegates();	// Unsubscribe from all events to this net socket

	void HandleClientConnection(StringHash eventType, VariantMap& eventData);
	void HandleClientIdentity(StringHash eventType, VariantMap& eventData);
};