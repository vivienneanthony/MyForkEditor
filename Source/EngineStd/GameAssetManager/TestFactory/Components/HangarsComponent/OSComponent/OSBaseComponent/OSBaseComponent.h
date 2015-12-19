#pragma once

#include "../OSInterface.h"

class OSBaseComponent : public OSInterface
{
	URHO3D_OBJECT(OSBaseComponent, OSInterface)

public:
	const static String g_Name;
	OSBaseComponent (Context* context);
	OSBaseComponent ();
	virtual ~OSBaseComponent ();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node* pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node* pData) { return true; }

public:
	// OSInterface interface
	// There can be more methods.
	virtual void Upgrade(VariantMap& materials);
	virtual void Destroy();

	virtual StationModules& GetAllModules();
	virtual StrongNodePtr GetModule(String name);
	virtual StationModules& GetModule(OSModuleType type);
	virtual int GetStationModuleCount();

	virtual void CreateModule(OSType& type);
	virtual void CreateModule(pugi::xml_node& pData);

	virtual void DestroyModule(String name);
	virtual void DestroyModule(OSModuleType type);
	virtual void DestroyModule(StrongNodePtr pModule);

	const OSType GetStationType() { return m_StationType; }
	void SetStationType(const OSType& value) { m_StationType = value; }

protected:
	StrongNodePtr CreateStationModule(pugi::xml_node& node);


protected:
	// Delegates
	virtual void VInitializeAllDelegates() { }
	virtual void VDestroyAllDelegates() { }

protected:
	StationModules m_StationModules;					// All modules that have this station

	StationModules	m_TempStationModules;				// If method get modules if called. We have to put all modules in this vector.

	OSType m_StationType;					// OriginStatio type
	String m_Name;										// Station name

	static unsigned int m_ModulesCount;				// How many modules have this station ?
};