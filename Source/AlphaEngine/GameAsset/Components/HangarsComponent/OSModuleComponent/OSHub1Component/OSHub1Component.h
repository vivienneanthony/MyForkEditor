#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSHub1Component : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSHub1Component, OSBaseModuleComponent)
public:
	const static String g_Name;
	OSHub1Component(Context* context);
	OSHub1Component();
	virtual ~OSHub1Component();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

	// OSHub1Component interface
	// There can be more methods.


protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

};