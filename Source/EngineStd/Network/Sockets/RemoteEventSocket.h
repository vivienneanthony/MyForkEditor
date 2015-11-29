#pragma once

#include "NetSocket.h"

class RemoteEventSocket : public NetSocket
{
	URHO3D_OBJECT(RemoteEventSocket, NetSocket)

public:
	enum
	{
		NetMsg_Event,
		NetMsg_PlayerLoginOk,
	};


	// client attach to server
	RemoteEventSocket(Context* context);
	virtual ~RemoteEventSocket();

};