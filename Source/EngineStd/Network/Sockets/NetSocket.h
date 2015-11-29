#pragma once

#include "../Packets/BasePacket.h"
#include "../Managers/BaseSocketManager.h"

class NetSocket : public Object
{
	URHO3D_OBJECT(NetSocket, Object);

	friend class BaseSocketManager;
	typedef List<SharedPtr<BasePacket>> PacketList;

public:
	NetSocket(Context* context);				// Handled by client
	NetSocket(Context* context, String hostIP);	// Handled by server
	virtual ~NetSocket();


	// Connect to the server. 
	// Identity VariantMap supplied as parameters when connecting. The identity data can contain for example the user name or credentials
	bool Connect(String ip, unsigned short port, Scene* scene, const VariantMap& map = Variant::emptyVariantMap);
	void Disconnect(int waitMSec = 0);

	// Simply add packet to end of list.  
	void Send(SharedPtr<BasePacket> packet);

	// These function you probably want to override
	// VHandleOutput - iterate thorugh output list and send packets.
	virtual void VHandleOutput();
	// VHandleInput - iterate thourgh input list and get packets.
	virtual void VHandleInput();

	// Some Urho3D wrappers
	inline void SetLogStatistic(bool enable) { m_pConnection->SetLogStatistics(enable); }
	inline void SetScene(Scene* scene) { m_pConnection->SetScene(scene); }
	inline void SetControls(Controls& control) { m_pConnection->SetControls(control); }
	inline void SetConnectPending(bool pending) { m_pConnection->SetConnectPending(pending); }

	inline bool IsConnected() { return m_pConnection->IsConnected(); }
	
	inline VariantMap& GetIdentity() { m_pConnection->GetIdentity(); }
	inline Connection* GetConnection() { return m_pConnection; }

	void HandleException() { m_DeleteFlags = true; }

	// Getters/Setters
	virtual bool VHasOutput() { return !m_OutList.Empty(); }
	unsigned int GetID() { return m_UniqueId; }
	String GetIpAddr() { return m_IpAddr; }
	bool IsInternal() { return m_bIsInternal; }
	String GetTimeCreated() { return m_TimeCreated; }
	int GetSendsOf() { return m_SendsOfs; }

protected:
	virtual void VInitializeDelegates();	// Subsribe to events to this net socket.
	virtual void VDestroyAllDelegates();	// Unsubscribe from all events to this net socket
	
	void HandleNetworkMessage(StringHash eventType, VariantMap& eventData);
	void HandleRemoteEventNetworkMessage(StringHash eventType, VariantMap& eventData);


protected:
	Network* m_pNetwork;		// Networking.
	Connection* m_pConnection;	// Client connection to the server.

	unsigned int m_UniqueId;	// Unique id that socket manager gives. 
	bool m_DeleteFlags;			// helps to handle reconnection, if a connection drops for a while.

	PacketList m_OutList;		// packets to send
	PacketList m_InList;		// packets just recieved

	String m_IpAddr;			// IP Address of remote machine
	unsigned int m_TimeOut;		// The socket timeout
	int m_SendsOfs;				// Tracking the output messages

	bool m_bIsInternal;			// is the remote IP is internal or external.
	String m_TimeCreated;		// Time when the socket was created
};