#include "EngineStd.h"
#include "BasePacket.h"


const PacketId INVALID_PACKET_ID = -1;

String BasePacket::g_Type = "BasePacket";

BasePacket::BasePacket(Context* context, PacketId id, bool isOrder) : IPacket(context)
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