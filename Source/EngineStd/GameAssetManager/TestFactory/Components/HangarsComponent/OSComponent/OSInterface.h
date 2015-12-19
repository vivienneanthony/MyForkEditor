#pragma once


#include "../../MainComponent.h"
#include "../OSModulesComponent/OSModuleInterface.h"

typedef Vector<StrongNodePtr> StationModules;

enum OSType
{
	BaseStation,
	OrcasStation,
	TerranStation,
	VordonsStation,
	OSUNKNOWN,
};

class OSInterface : public MainComponent
{
public:
	OSInterface(Context* context);
	OSInterface();
	virtual ~OSInterface();
	
	// These should be implemented by specific OS
	virtual void Upgrade(VariantMap& materials) = 0;
	virtual void Destroy() = 0;

	virtual StationModules& GetAllModules() = 0;
	virtual StrongNodePtr GetModule(String name) = 0;
	virtual StationModules& GetModule(OSModuleType type) = 0;
	virtual int GetStationModuleCount() = 0;
	
	virtual void CreateModule(OSType& type) = 0;
	virtual void CreateModule(pugi::xml_node& pData) = 0;

	virtual void DestroyModule(String name) = 0;
	virtual void DestroyModule(OSModuleType type) = 0;
	virtual void DestroyModule(StrongNodePtr pModule) = 0;

	virtual const OSType GetStationType() = 0;
	virtual void SetStationType(const OSType& type) = 0;
};