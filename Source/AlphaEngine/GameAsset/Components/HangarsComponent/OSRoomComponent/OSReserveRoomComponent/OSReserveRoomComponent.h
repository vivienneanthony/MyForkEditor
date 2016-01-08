#pragma once

#include "../OSBaseRoomComponent/OSBaseRoomComponent.h"

class OSReserveRoomComponent : public OSBaseRoomComponent
{
	URHO3D_OBJECT(OSReserveRoomComponent, OSBaseRoomComponent)
public:
	const static String g_Name;
	OSReserveRoomComponent (Context* context);
	OSReserveRoomComponent ();
	virtual ~OSReserveRoomComponent ();

	// OSBaseRoomComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

public:
	// Specific OSReserveRoomComponent
	void SetIronCount(int value) { m_IronCount = value; }
	int GetIronCount() { return m_IronCount; }

protected:
	int m_IronCount;					// Just for example.
};