#pragma once

#include "../../MainComponent.h"

class ChemicalComponent : public MainComponent
{
	URHO3D_OBJECT(ChemicalComponent, MainComponent)


public:
	const static String g_Name;
	
	ChemicalComponent(Context* context);
	ChemicalComponent();
	virtual ~ChemicalComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root) { return root; }

	virtual String VGetName() const { return g_Name; }

protected:
	
};