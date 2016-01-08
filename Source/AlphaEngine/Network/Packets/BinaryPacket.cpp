#include "AlphaEngineStd.h"
#include "BinaryPacket.h"

String BinaryPacket::g_Type = "BinaryPacket";

BinaryPacket::BinaryPacket(Context* context, PacketId id, unsigned char* data, unsigned int size, bool ordered, bool realiable, int unsignedContentId) : BasePacket(context, id, ordered)
{
	Urho3D::PODVector<unsigned char> buffer = Urho3D::PODVector<unsigned char>(data, size);
	m_Buffer.SetData(buffer);

	m_Size = size;
	m_bIsRealiable = realiable;
	m_UnsignedContentId = unsignedContentId;
}

BinaryPacket::BinaryPacket(Context* context, PacketId id, VectorBuffer& buffer, bool ordered, bool realiable, int unsignedContentId) : BasePacket(context, id, ordered)
{
	m_Buffer = buffer;

	m_bIsRealiable = realiable;
	m_UnsignedContentId = unsignedContentId;
}

BinaryPacket::~BinaryPacket()
{

}