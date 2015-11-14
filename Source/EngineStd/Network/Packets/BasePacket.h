#pragma once

class BasePacket : public IPacket
{
	URHO3D_OBJECT(BasePacket, IPacket)

public:
	BasePacket(Context* context, unsigned int id, bool isOrder);
	virtual ~BasePacket();

	virtual String VGetType() const { return "BasePacket"; }
	virtual VectorBuffer& VGetBuffer() { return m_Buffer; }
	virtual VariantMap& VGetData() { return m_EventData; }
	virtual unsigned int VGetSize() const {return m_Size; }
	virtual bool VIsBinary() = 0;

	// Getters/Setters
	inline unsigned int GetPacketId() { return m_PacketId; }
	inline const bool IsOrderered() { return m_bIsOrdered; }

public:
	VectorBuffer m_Buffer;
	int m_Size;

	VariantMap m_EventData;
	
	unsigned int m_PacketId;				// In order to specify message. It helps to handle specific message, if needed.
	bool m_bIsOrdered;						// For high performance, consider using unordered messages,
											// because for in-order messages there is only a single channel within the connection, 
											// and all previous in-order messages must arrive first before a new one can be processed.
};