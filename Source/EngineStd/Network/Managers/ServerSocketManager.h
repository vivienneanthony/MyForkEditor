#pragma once

#include "BaseSocketManager.h"

// ServerSocketManager.
// Alghoritm of ServerSocketManager:
// 1)Initialize server socket manager and attach a listen socket.
// 2)Call DoSelect() on the server socket manager.
// 3)It there is input on the listen socket,  create new socket and attach it to the socket manager.
// 4)Handle input/output socket.

class ServerSocketManager : public BaseSocketManager
{
	URHO3D_OBJECT(ServerSocketManager, BaseSocketManager)
public:
	ServerSocketManager(Context* context, String hostName, unsigned short portName);
	virtual ~ServerSocketManager();


};