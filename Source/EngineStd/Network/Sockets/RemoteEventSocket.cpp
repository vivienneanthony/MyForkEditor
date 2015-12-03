#include "EngineStd.h"

#include "../Packets/EventPacket.h"

#include "RemoteEventSocket.h"


RemoteEventSocket::RemoteEventSocket(Context* context, Connection* connection, String hostIp) : NetSocket(context, hostIp)
{
	m_pConnection = connection;
}

RemoteEventSocket::RemoteEventSocket(Context* context) : NetSocket(context)
{


}

RemoteEventSocket::~RemoteEventSocket()
{

}

void RemoteEventSocket::VHandleInput()
{
	NetSocket::VHandleInput();

	// traverse the list of m_InList packets and do something useful with them
	while (!m_InList.Empty())
	{
		SharedPtr<BasePacket> packet = *m_InList.Begin();
		m_InList.PopFront();
		if (!String::Compare(packet->VGetType().CString(), EventPacket::g_Type.CString(), false))
		{
			VariantMap data = packet->VGetData();

			SendEvent(data["EVENT_TYPE"].GetStringHash(), data);
		}
		else
		{
			URHO3D_LOGERROR("Failed to recognize event type.");
		}
	}
}
