#include "AlphaEngineStd.h"

#include "BaseSocketManager.h"

#include "Network/Packets/EventPacket.h"
#include "NetworkEventForwarder.h"


NetworkEventForwarder::NetworkEventForwarder(unsigned int connectionId) : Object(g_pApp->GetContext())
{
	m_ConnectionId = connectionId;
}


void NetworkEventForwarder::ForwardEventDelegate(StringHash eventType, VariantMap& eventData)
{
	SharedPtr<BasePacket> eventPacket(new EventPacket(context_, eventData["EVENT_TYPE"].GetStringHash(), eventData["EVENT_ORDER"].GetBool(), eventData));
	g_pSocketManager->Send(m_ConnectionId, eventPacket);
}