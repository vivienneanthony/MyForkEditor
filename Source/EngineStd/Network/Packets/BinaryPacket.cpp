#include "EngineStd.h"
#include "BinaryPacket.h"


BinaryPacket::BinaryPacket(Context* context, unsigned int id, unsigned char* data, unsigned int size, bool ordered, bool realiable, int unsignedContentId) : BasePacket(context, id, ordered)
{
	Urho3D::PODVector<unsigned char> buffer = Urho3D::PODVector<unsigned char>(data, size);
	m_Buffer.SetData(buffer);

	m_Size = size;
	m_bIsRealiable = realiable;
	m_UnsignedContentId = unsignedContentId;
}

BinaryPacket::BinaryPacket(Context* context, unsigned int id, VectorBuffer& buffer, bool ordered, bool realiable, int unsignedContentId) : BasePacket(context, id, ordered)
{
	m_Buffer = buffer;

	m_bIsRealiable = realiable;
	m_UnsignedContentId = unsignedContentId;
}

BinaryPacket::~BinaryPacket()
{

}