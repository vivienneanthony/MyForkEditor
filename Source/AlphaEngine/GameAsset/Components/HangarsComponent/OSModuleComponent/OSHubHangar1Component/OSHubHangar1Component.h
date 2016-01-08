#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSHubHangar1Component : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSHubHangar1Component, OSBaseModuleComponent)
public:
	const static String g_Name;
	OSHubHangar1Component(Context* context);
	OSHubHangar1Component();
	virtual ~OSHubHangar1Component();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

};