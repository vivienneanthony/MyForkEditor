#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSHubCoreComponent : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSHubCoreComponent, OSBaseModuleComponent)
public:
	const static String g_Name;
	OSHubCoreComponent(Context* context);
	OSHubCoreComponent();
	virtual ~OSHubCoreComponent();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

	// OSHubCoreComponent interface
	// There can be more methods.
	void SetComputersCount(const int value) { m_ComputerCount = value; }
	int GetComputersCount() const { return m_ComputerCount; }

protected:
	virtual bool VDelegateInit(pugi::xml_node pData);

protected:
	// OSHubCoreComponent information
	int m_ComputerCount;							// How many computers has

};