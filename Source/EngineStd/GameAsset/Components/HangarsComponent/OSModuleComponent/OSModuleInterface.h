#pragma once


#include "../../MainComponent.h"

enum OSModuleType
{
	HubCore,
	Hub1,
	HubHangar1,
	HubHangar2,
	HubCommArray2,
	TrussTriple1,
	ModuleUNKNOWN,
};

class OSModuleInterface : public MainComponent
{
	URHO3D_OBJECT(OSModuleInterface, MainComponent)
public:
	OSModuleInterface(Context* context);
	OSModuleInterface();
	virtual ~OSModuleInterface();

	// OS module interface
	// These should be implemented by station modules
	virtual void SetLevel(int value) = 0;
	virtual int GetLevel() = 0;
		
	virtual void Upgrade(VariantMap& materials) = 0;
	virtual void Destroy() = 0;

	virtual OSModuleType GetModuleType() = 0;
	virtual void SetModuleType(OSModuleType type) = 0;
};