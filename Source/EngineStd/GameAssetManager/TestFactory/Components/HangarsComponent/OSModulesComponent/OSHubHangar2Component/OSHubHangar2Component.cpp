#include "EngineStd.h"
#include "OSHubHangar2Component.h"

const String OSHubHangar2Component::g_Name = String("OSHubHangar2Component");

OSHubHangar2Component::OSHubHangar2Component(Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = HubHangar2;
}

OSHubHangar2Component::OSHubHangar2Component() : OSBaseModuleComponent()
{
	m_ModuleType = HubHangar2;
}

OSHubHangar2Component::~OSHubHangar2Component()
{

}

// mainComponent interface
bool OSHubHangar2Component::VDelegateInit(pugi::xml_node* pData)
{


	return true;
}

void OSHubHangar2Component::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSHubHangar2Component::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSHubHangar2Component::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}