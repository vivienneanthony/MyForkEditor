#include "EngineStd.h"
#include "NetSocket.h"
#include "../Packets/BinaryPacket.h"
#include "../Packets/EventPacket.h"

NetSocket::NetSocket(Context* context) : Object(context)
{
	m_pNetwork = GetSubsystem<Network>();
	m_DeleteFlags = 0;
	m_SendsOfs = 0;
	m_TimeOut = 0;
	m_IpAddr = "remote";
	m_bIsInternal = false;
	m_pConnection = NULL;

	VInitializeDelegates();
}

NetSocket::NetSocket(Context* context, String hostIP) : Object(context)
{
	m_pNetwork = GetSubsystem<Network>();
	m_DeleteFlags = 0;
	m_SendsOfs = 0;
	m_TimeOut = 0;

	m_IpAddr = hostIP;
	m_bIsInternal = true;

	VInitializeDelegates();
}

NetSocket::~NetSocket()
{
	if (m_pConnection)
	{
		if (m_pConnection->IsConnected() && m_pConnection->IsClient())
		{
			m_pConnection->Disconnect(1000);
		}
	}

	VDestroyAllDelegates();
}

bool NetSocket::Connect(String ip, unsigned short port, Scene* scene, const VariantMap& map)
{
	// Connect to server, specify scene to use as a client for replication
	m_UniqueId = 0; // Reset own object ID from possible previous connection
	if (m_pNetwork->Connect(ip, port, scene, map))
	{
		m_TimeCreated = g_pApp->GetTimer()->GetTimeStamp();
		URHO3D_LOGDEBUG(String("Socket was connected to the server. ") + m_TimeCreated);
	}
	else
	{
		URHO3D_LOGDEBUG(String("Socket connection to the server was failed."));
		return false;
	}

	m_pConnection = m_pNetwork->GetServerConnection();
	return true;
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
			
			m_pConnection->SendRemoteEvent(eventNode, eventType, eventpkt->IsOrderered(), eventData);
			fSent++;
		}

		m_OutList.PopFront();

	} while (fSent && !m_OutList.Empty());
	
}

void NetSocket::VHandleInput()
{

}

// DELEGATES

void NetSocket::VInitializeDelegates()
{
	SubscribeToEvent(E_NETWORKMESSAGE, URHO3D_HANDLER(NetSocket, HandleNetworkMessage));
	SubscribeToEvent(E_REMOTEEVENTDATA, URHO3D_HANDLER(NetSocket, HandleRemoteEventNetworkMessage));
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
			eventpacket = SharedPtr<BasePacket>(new EventPacket(context_, packetId, eventType, false, eventData));
		}
		else
		{
			eventpacket = SharedPtr<BasePacket>(new EventPacket(context_, packetId, node, eventType, false, eventData));
		}

		if (eventpacket.NotNull())
		{
			m_OutList.Push(eventpacket);
		}
	}
}