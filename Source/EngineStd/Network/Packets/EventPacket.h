#pragma once

#include "BasePacket.h"

class EventPacket : public BasePacket
{
	URHO3D_OBJECT(EventPacket, BasePacket);

public:
	EventPacket(Context* context, unsigned int id, Node* node, StringHash eventType, bool isOrder = false, const VariantMap &eventData = Variant::emptyVariantMap); // Remote event packet with the specified node as sender. 
	EventPacket(Context* context, unsigned int id, StringHash eventType, bool isOrder = false, const VariantMap &eventData = Variant::emptyVariantMap); // Remote event packet without the specified node as sender. 

	virtual ~EventPacket() { }

	// IPacket interface methods
	virtual String VGetType() const { return String("EventPacket"); }
	virtual VectorBuffer& VGetBuffer() { return m_Buffer; }
	virtual VariantMap& VGetData() { return m_EventData; }
	virtual unsigned int VGetSize() const { return m_Size; }
	virtual bool VIsBinary() { return true; }

	//Getters/Setters
	inline Node* GetNode() { return m_pNode; }
	inline const StringHash GetEventType() { return m_EventType; }

	inline void SetEventData(VariantMap& data) { m_EventData = data; }

protected:
	Urho3D::Node* m_pNode;
	StringHash m_EventType;

};