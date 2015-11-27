#pragma once

//---------------------------------------------------------------------------------------------------------------------
// IEventData     
// Base type for event object hierarchy, may be used itself for simplest event notifications such as those that do 
// not carry additional payload data. If any event needs to propagate with payload data it must be defined separately.
//---------------------------------------------------------------------------------------------------------------------

class IEventData;
typedef SharedPtr<IEventData> IEventDataPtr;

class IEventData : public Object
{
	URHO3D_OBJECT(IEventData, Object);
public:
	IEventData(Context* context);
	virtual ~IEventData(void) {}
	virtual const EventType& VGetEventType(void) const = 0;
	virtual float GetTimeStamp(void) const = 0;
	virtual const VariantMap& VSerialize() = 0;
	virtual void VDeserialize(VariantMap& in) = 0;
	virtual IEventDataPtr VCopy(void) = 0;
	virtual const String GetName(void) const = 0;
};
