#include "EngineStd.h"
#include "EventPacket.h"

String EventPacket::g_Type = "EventPacket";

EventPacket::EventPacket(Context* context, Node* node, StringHash eventType, bool isOrder, const VariantMap &eventData) : BasePacket(context, INVALID_PACKET_ID, isOrder)
{
	m_pNode = node;
	m_EventType = eventType;
	m_EventData = eventData;
	m_Size = m_EventData.Size();
}

EventPacket::EventPacket(Context* context, StringHash eventType, bool isOrder, const VariantMap &eventData) : BasePacket(context, INVALID_PACKET_ID, isOrder)
{
	m_pNode = nullptr;
	m_EventType = eventType;
	m_EventData = eventData;
	m_Size = m_EventData.Size();
}