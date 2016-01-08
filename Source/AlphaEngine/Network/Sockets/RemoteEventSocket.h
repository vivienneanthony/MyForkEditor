#pragma once

#include "NetSocket.h"

class RemoteEventSocket : public NetSocket
{
	URHO3D_OBJECT(RemoteEventSocket, NetSocket)

public:
	// server accepting a client
	RemoteEventSocket(Context* context, Connection* connection, String hostIp);

	// client attach to server
	RemoteEventSocket(Context* context);
	virtual ~RemoteEventSocket();

	virtual void VHandleInput();

protected:
	void CreateEngineEvent(VariantMap& data);


};