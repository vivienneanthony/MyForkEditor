#pragma once

// BaseSocketManager - Sockets need a socket manager, whether they are on client or on a server.
// Organize multiple sockets into a manageable group, take care of handling the initialization
// and shutdown of the sockets systems and provides some useful utilities.
// BaseClass for more specialized socket managers for servers and client.

#include "../Sockets/NetSocket.h"

class NetSocket;

class BaseSocketManager : public Object
{
	URHO3D_OBJECT(BaseSocketManager, Object);

	typedef List<NetSocket*> SocketList;
	typedef HashMap<unsigned int, NetSocket*> SocketIdMap;
	
public:
	BaseSocketManager(Context* context);
	virtual ~BaseSocketManager();

	bool Init();
	void Shutdown();

	int AddSocket(NetSocket* socket);
	void RemoveSocket(NetSocket* socket);

	bool Send(unsigned int sockId, SharedPtr<BasePacket> packet);
	void DoSelect(bool handleInput = true);

	// Getters/Setters

protected:
	virtual void VInitializeDelegates();	// Subsribe to events for this socket manager
	virtual void VDestroyAllDelegates();		// Unsubscribe from all events for this socket manager

protected:
	Network* m_pNetwork;				
	
	SocketList m_SocketsList;				// a list of sockets/connection
	SocketIdMap m_SocketsMap;				// a map from uintegers ID's to socket handles/connection

	unsigned int m_NextSocketId;			// Now it is simple ticker for the next socket ID. Guarantee a unique ID for each socket in the system
											// Instead of this can be such as a player id or an account ID number or whatever
											// which be assigned by logging system, when new accounts were created
												

	unsigned int m_Inbound;					// staticstic gathering - inbound data
	unsigned int m_Outbound;				// staticstic gathering - outbound data
	unsigned int m_MaxOpenSockets;			// staticstic gathering - max open sockets

	unsigned int m_SubnetMask;				// the subnet mask of the interal network
	unsigned int m_Subnet;					// the subnet of the internal network
	
	NetSocket* FindSocket(unsigned int sockId);

};

extern BaseSocketManager* g_pSocketManager;