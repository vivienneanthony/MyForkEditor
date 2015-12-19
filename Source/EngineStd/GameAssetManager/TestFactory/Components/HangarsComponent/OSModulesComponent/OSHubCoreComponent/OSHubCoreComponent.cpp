#include "EngineStd.h"
#include "OSHubCoreComponent.h"

const String OSHubCoreComponent::g_Name = String("OSHubCoreComponent");

OSHubCoreComponent::OSHubCoreComponent(Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = HubCore;
}

OSHubCoreComponent::OSHubCoreComponent() : OSBaseModuleComponent()
{
	m_ModuleType = HubCore;
}

OSHubCoreComponent::~OSHubCoreComponent()
{

}

// mainComponent interface
bool OSHubCoreComponent::VDelegateInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("HubCoreInformation");
	if (node)
	{
		pugi::xml_node node = pData->child("Computers");
		m_ComputerCount = node.attribute("count").as_int();
	}

	return true;
}

void OSHubCoreComponent::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSHubCoreComponent::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSHubCoreComponent::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}