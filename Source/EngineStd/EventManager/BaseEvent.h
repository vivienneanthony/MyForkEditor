#pragma once

//---------------------------------------------------------------------------------------------------------------------
// class BaseEventData	
//---------------------------------------------------------------------------------------------------------------------

class BaseEventData : public IEventData
{
public:
	explicit BaseEventData(const float timeStamp = 0.0f, bool eventOrder = false) : 
		 m_TimeStamp(timeStamp) 
		,m_EventOrder(eventOrder)
		,m_OutSerializer(Variant::emptyVariantMap)
		,IEventData(g_pApp->GetContext())
	{
	}
	
	float VGetTimeStamp(void) const { return m_TimeStamp; }

	// Serializing for network input / output
	virtual VariantMap& VSerialize() { return m_OutSerializer; }
	virtual void VDeserialize(VariantMap& in) { }

	// Getters/Setters
	bool GetEventOrder() { return m_EventOrder; }

protected:
	const float m_TimeStamp;
	bool m_EventOrder;

	VariantMap m_OutSerializer;
};
