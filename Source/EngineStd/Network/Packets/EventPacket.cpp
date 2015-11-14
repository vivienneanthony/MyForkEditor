#include "EngineStd.h"
#include "EventPacket.h"

EventPacket::EventPacket(Context* context, unsigned int id, Node* node, StringHash eventType, bool isOrder, const VariantMap &eventData) : BasePacket(context, id, isOrder)
{
	m_pNode = node;
	m_EventType = eventType;
	m_EventData = eventData;
	m_Size = m_EventData.Size();
}

EventPacket::EventPacket(Context* context, unsigned int id, StringHash eventType, bool isOrder, const VariantMap &eventData) : BasePacket(context, id, isOrder)
{
	m_pNode = nullptr;
	m_EventType = eventType;
	m_EventData = eventData;
	m_Size = m_EventData.Size();
}