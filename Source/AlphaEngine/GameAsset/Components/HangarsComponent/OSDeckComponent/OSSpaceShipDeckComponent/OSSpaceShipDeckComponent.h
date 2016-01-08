#pragma once

#include "../OSBaseDeckComponent/OSBaseDeckComponent.h"

class OSSpaceShipDeckComponent : public OSBaseDeckComponent
{
	URHO3D_OBJECT(OSSpaceShipDeckComponent, OSBaseDeckComponent)
public:
	const static String g_Name;
	OSSpaceShipDeckComponent (Context* context);
	OSSpaceShipDeckComponent ();
	virtual ~OSSpaceShipDeckComponent ();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

public:
	// Specific OSSpaceShipDeckComponent 
	void SetSpaceShipCount(int value) { m_SpaceShipCount = value; }
	int GetSpaceShipCount() { return m_SpaceShipCount; }

protected:
	int m_SpaceShipCount;						//	How much space ships can have this deck
};