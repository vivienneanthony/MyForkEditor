#pragma once

typedef unsigned int ConnectionId;
extern const ConnectionId INVALID_CONNECTION_ID;

typedef unsigned int PacketId;
extern const PacketId INVALID_PACKET_ID;

class IPacket : public Object
{
	URHO3D_OBJECT(IPacket, Object)
public:
	IPacket(Context* context);
	virtual ~IPacket();

	virtual String VGetType() const = 0;
	virtual VectorBuffer& VGetBuffer() = 0;
	virtual VariantMap& VGetData() = 0;
	virtual unsigned int VGetSize() const = 0;
	virtual bool VIsBinary() = 0;
};