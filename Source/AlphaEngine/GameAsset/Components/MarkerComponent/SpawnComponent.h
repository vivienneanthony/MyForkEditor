#pragma once

#include "../MainComponent.h"

class SpawnComponent : public MainComponent
{
	URHO3D_OBJECT(SpawnComponent, MainComponent)

public:
	const static String g_Name;
	
	SpawnComponent(Context* context);
	SpawnComponent();
	virtual ~SpawnComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

	
protected:
	
protected:
	
};
