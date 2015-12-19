#include "EngineStd.h"
#include "OSHubCommArray2Component.h"

const String OSHubCommArray2Component::g_Name = String("OSHubCommArray2Component");

OSHubCommArray2Component::OSHubCommArray2Component(Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = HubCommArray2;
}

OSHubCommArray2Component::OSHubCommArray2Component() : OSBaseModuleComponent()
{
	m_ModuleType = HubCommArray2;
}

OSHubCommArray2Component::~OSHubCommArray2Component()
{

}

// mainComponent interface
bool OSHubCommArray2Component::VDelegateInit(pugi::xml_node* pData)
{

	return true;
}

void OSHubCommArray2Component::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSHubCommArray2Component::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSHubCommArray2Component::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}