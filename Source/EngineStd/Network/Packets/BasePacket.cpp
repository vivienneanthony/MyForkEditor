#include "EngineStd.h"
#include "BasePacket.h"

BasePacket::BasePacket(Context* context, unsigned int id, bool isOrder) : IPacket(context)
{
	m_Buffer = VectorBuffer(Variant::emptyBuffer);
	m_Size = 0;

	m_EventData = VariantMap(Variant::emptyVariantMap);

	m_PacketId = id;
	m_bIsOrdered = isOrder;
}


BasePacket::~BasePacket()
{

}