#include "EngineStd.h"
#include "Network/Packets/EventPacket.h"
#include "NetworkEventForwarder.h"


NetworkEventForwarder::NetworkEventForwarder(Connection* connection) : Object(g_pApp->GetContext())
{
	m_pConnection = connection;
}


void NetworkEventForwarder::ForwardEventDelegate(StringHash eventType, VariantMap& eventData)
{
	
	

}