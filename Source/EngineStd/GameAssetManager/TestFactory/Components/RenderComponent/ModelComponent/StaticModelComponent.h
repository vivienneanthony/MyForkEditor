#pragma once

#include "../../MainComponent.h"

class StaticModelComponent : public MainComponent
{
	URHO3D_OBJECT(StaticModelComponent, MainComponent)

public:
	const static String g_Name;
	StaticModelComponent(Context* context);
	StaticModelComponent();
	virtual ~StaticModelComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node* pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

protected:
	StaticModel* m_pModel;

	String m_ModelResource;
	String m_MaterialResource;

	bool m_bIsApplyMaterial;
	bool m_CastShadow;
	unsigned int m_LightMask;
	bool m_bCastShadow;

	static unsigned int m_ModelCount;				// How many model has this node ?
};