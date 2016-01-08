#include "AlphaEngineStd.h"
#include "NetSocket.h"

#include "GameLogic/BaseGameLogic.h"

#include "../Packets/BinaryPacket.h"
#include "../Packets/EventPacket.h"


const ConnectionId INVALID_CONNECTION_ID = -1;

NetSocket::NetSocket(Context* context) : Object(context)
{
	m_pNetwork = g_pApp->GetNetwork();
	m_DeleteFlags = 0;
	m_SendsOfs = 0;
	m_TimeOut = 0;
	m_IpAddr = "remote";
	m_bIsInternal = false;
	m_pConnection = NULL;

	VInitializeAllDelegates();
	
}

NetSocket::NetSocket(Context* context, String hostIP) : Object(context)
{	
	m_pNetwork = g_pApp->GetNetwork();
	m_DeleteFlags = 0;
	m_SendsOfs = 0;
	m_TimeOut = 0;

	m_IpAddr = hostIP;
	m_bIsInternal = true;

	m_pConnection = NULL;

	VInitializeAllDelegates();
}

NetSocket::~NetSocket()
{
	m_pConnection = m_pNetwork->GetServerConnection();
	if (m_pConnection)
	{
		m_pConnection->Disconnect();
	}

	VDestroyAllDelegates();
}



bool NetSocket::Connect(String ip, unsigned short port, Scene* scene, const VariantMap& map)
{
	assert(m_pNetwork);

	// Connect to server, specify scene to use as a client for replication
	m_UniqueId = 0; // Reset own object ID from possible previous connection
	if (m_pNetwork->Connect(ip, port, scene, map))
	{
		m_TimeCreated = g_pApp->GetTimer()->GetTimeStamp();
		URHO3D_LOGDEBUG(String("Socket try to connect to the server. ") + m_TimeCreated);
	}
	else
	{
		URHO3D_LOGDEBUG(String("Socket connection to the server was failed."));
		return false;
	}

	m_pConnection = m_pNetwork->GetServerConnection();
	
	if (!m_pConnection)
	{
		return false;
	}

	return m_pConnection->IsConnectPending();
}

void NetSocket::Disconnect(int waitMSec)
{
	m_pConnection->Disconnect(waitMSec);
	URHO3D_LOGDEBUG(String("Socket was disconnected to the server after ") + String(waitMSec) + String("Msec"));
}

void NetSocket::Send(SharedPtr<BasePacket> packet)
{
	m_OutList.Push(packet);
}

void NetSocket::VHandleOutput()
{
	int fSent = 0;
	do
	{
		assert(!m_OutList.Empty());
		PacketList::Iterator it = m_OutList.Begin();

		SharedPtr<BasePacket> pkt = *it;
		if (pkt->VIsBinary())
		{
			// Safe cast. Exist only one type of binary packet.
			SharedPtr<BinaryPacket> binarypkt = DynamicCast<BinaryPacket>(pkt);
			VectorBuffer& buffer = binarypkt->VGetBuffer();
			int size = binarypkt->VGetSize();
			bool order = binarypkt->IsOrderered();
			bool reliable = binarypkt->IsReliable();
			int contentId = binarypkt->GetContentId();

			m_pConnection->SendMessage(binarypkt->GetPacketId(), reliable, order, buffer, contentId);
			fSent++;
		}
		else
		{
			// Safe cast. Exist only one type of event packet.
			SharedPtr<EventPacket> eventpkt = DynamicCast<EventPacket>(pkt);
			VariantMap& eventData = eventpkt->VGetData();
			StringHash eventType = eventpkt->GetEventType();
			Node* eventNode = eventpkt->GetNode();
			eventData["EVENT_PACKET_ID"] = StringHash(eventpkt->GetPacketId());
			if (eventNode)
			{
				m_pConnection->SendRemoteEvent(eventNode, eventType, eventpkt->IsOrderered(), eventData);
			}
			else
			{
				m_pConnection->SendRemoteEvent(eventType, eventpkt->IsOrderered(), eventData);
			}

			fSent++;
		}

		m_OutList.PopFront();

	} while (fSent && !m_OutList.Empty());
	
}

void NetSocket::VHandleInput()
{

}

// DELEGATES

void NetSocket::VInitializeAllDelegates()
{
	SubscribeToEvent(E_NETWORKMESSAGE, URHO3D_HANDLER(NetSocket, HandleNetworkMessage));
	SubscribeToEvent(E_REMOTEEVENTDATA, URHO3D_HANDLER(NetSocket, HandleRemoteEventNetworkMessage));
	SubscribeToEvent(E_CLIENTDISCONNECTED, URHO3D_HANDLER(NetSocket, HandleDisconnect));

}

void NetSocket::VDestroyAllDelegates()
{
	UnsubscribeFromAllEvents();
}

void NetSocket::HandleNetworkMessage(StringHash eventType, VariantMap& eventData)
{
	Connection* connection = (Connection*)eventData[Urho3D::RemoteEventData::P_CONNECTION].GetPtr();
	unsigned int packetId = eventData[Urho3D::NetworkMessage::P_MESSAGEID].GetUInt();
	
	const PODVector<unsigned char>& data = eventData[Urho3D::NetworkMessage::P_DATA].GetBuffer();
	VectorBuffer buffer = VectorBuffer(data);
	if (connection)
	{
		SharedPtr<BasePacket> binarypacket = SharedPtr<BasePacket>(nullptr);
		binarypacket = SharedPtr<BasePacket>(new BinaryPacket(context_, packetId, buffer));
		m_InList.Push(binarypacket);
	}
}

void NetSocket::HandleRemoteEventNetworkMessage(StringHash eventType, VariantMap& eventData)
{
	Connection* connection = (Connection*)eventData[Urho3D::RemoteEventData::P_CONNECTION].GetPtr();
	if (connection)
	{
		SharedPtr<BasePacket> eventpacket = SharedPtr<BasePacket>(nullptr);
		Node* node = (Node*)connection->GetEventSender();
		VariantMap& eventData = connection->GetEventDataMap();
		unsigned int packetId = eventData["EVENT_PACKET_ID"].GetUInt();
		
		if (!node)
		{
			eventpacket = SharedPtr<BasePacket>(new EventPacket(context_, eventType, false, eventData));
		}
		else
		{
			eventpacket = SharedPtr<BasePacket>(new EventPacket(context_, node, eventType, false, eventData));
		}

		if (eventpacket.NotNull())
		{
			m_OutList.Push(eventpacket);
		}
	}
}

void NetSocket::HandleDisconnect(StringHash eventType, VariantMap& eventData)
{
	String reason = eventData["REASON"].GetString();
	
	if (reason == String("Incorrect_password"))
	{

	}
	else if (reason == String("Incorrect_login"))
	{

	}
}
