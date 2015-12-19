#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSHubHangar2Component : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSHubHangar2Component, OSBaseModuleComponent)
public:
	const static String g_Name;
	OSHubHangar2Component(Context* context);
	OSHubHangar2Component();
	virtual ~OSHubHangar2Component();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }


protected:
	virtual bool VDelegateInit(pugi::xml_node* pData);


};