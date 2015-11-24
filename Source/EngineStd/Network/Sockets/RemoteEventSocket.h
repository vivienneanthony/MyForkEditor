#pragma once

#include "NetSocket.h"

class RemoteEventSocket : public NetSocket
{
	URHO3D_OBJECT(RemoteEventSocket, NetSocket)

public:
	RemoteEventSocket(Context* context);
	virtual ~RemoteEventSocket();

};