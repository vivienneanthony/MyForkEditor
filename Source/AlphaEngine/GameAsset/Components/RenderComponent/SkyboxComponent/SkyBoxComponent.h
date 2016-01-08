#pragma once

#include "../../MainComponent.h"

class SkyBoxComponent : public MainComponent
{
	URHO3D_OBJECT(SkyBoxComponent, MainComponent)

public:
	const static String g_Name;
	SkyBoxComponent(Context* context);
	SkyBoxComponent();
	virtual ~SkyBoxComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

protected:
	String m_SkyboxModel;							// Skybox Model
	String m_SkyboxMaterial;
	
	Skybox* m_pNodeSkybox;						    // Save associated model
	
	bool m_bIsApplyMaterial;

	static unsigned int m_SkyboxCount;				// How many model has this node ?


};