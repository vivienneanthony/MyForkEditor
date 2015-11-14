#pragma once

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