#pragma once

#include "../../MainComponent.h"

#include "Chemistry/RawMaterial/RawMaterialData.h"

class RawMaterialComponent : public MainComponent
{
	URHO3D_OBJECT(RawMaterialComponent, MainComponent)

public:
	const static String g_Name;
	
	RawMaterialComponent(Context* context);
	RawMaterialComponent();
	virtual ~RawMaterialComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root) { return root; }

	virtual String VGetName() const { return g_Name; }

	// Raw Material component methods
	void SetRawMaterialData(const RawMaterialData& info) { m_RawMaterialData = info; }
	const RawMaterialData& GetRawMaterialData() { return m_RawMaterialData; }

protected:
	WeakPtr<IGameChemistry> m_pGameChemistry;

	RawMaterialData m_RawMaterialData;
	int m_Count;									// How much raw material ?
};