#pragma once

#include "BasePacket.h"

class BinaryPacket : public BasePacket
{
	URHO3D_OBJECT(BinaryPacket, BasePacket);

public:
	static String g_Type;

	BinaryPacket(Context* context, PacketId id, unsigned char* data, unsigned int size, bool ordered = false, bool realiable = true, int unsignedContentId = 0); // binary packet
	BinaryPacket(Context* context, PacketId id, VectorBuffer& buffer, bool ordered = false, bool realiable = true, int unsignedContentId = 0); // binary packet

	virtual ~BinaryPacket();

	// IPacket interface methods
	virtual String VGetType() const { return String("BinaryPacket"); }
	virtual VectorBuffer& VGetBuffer() { return m_Buffer; }
	virtual VariantMap& VGetData() { return m_EventData; }
	virtual unsigned int VGetSize() const { return m_Size; }
	virtual bool VIsBinary() { return true; }

	// Getters/Setters
	inline const int GetContentId() { return m_UnsignedContentId; }
	inline const bool IsReliable() { return m_bIsRealiable; }

	inline void SetBuffer(VectorBuffer& buffer, int size) { m_Buffer = buffer; m_Size = size; }

protected:
	int m_UnsignedContentId;	// When called SendMassage function, we pass this argument.
	bool m_bIsRealiable;		// When called SendMassage function, we pass this argument.
};