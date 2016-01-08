#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSHubCommArray2Component : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSHubCommArray2Component, OSBaseModuleComponent)
public:
	const static String g_Name;
	OSHubCommArray2Component(Context* context);
	OSHubCommArray2Component();
	virtual ~OSHubCommArray2Component();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

	// OSHubCommArray2Component interface
	// There can be more methods.

protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

};