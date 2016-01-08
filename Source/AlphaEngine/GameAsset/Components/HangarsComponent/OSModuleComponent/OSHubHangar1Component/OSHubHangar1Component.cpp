#include "AlphaEngineStd.h"
#include "OSHubHangar1Component.h"

const String OSHubHangar1Component::g_Name = String("OSHubHangar1Component");

OSHubHangar1Component::OSHubHangar1Component(Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = HubHangar1;
}

OSHubHangar1Component::OSHubHangar1Component() : OSBaseModuleComponent()
{
	m_ModuleType = HubHangar1;
}

OSHubHangar1Component::~OSHubHangar1Component()
{

}

// mainComponent interface
bool OSHubHangar1Component::VDelegateInit(pugi::xml_node pData)
{

	return true;
}

void OSHubHangar1Component::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSHubHangar1Component::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSHubHangar1Component::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}

pugi::xml_node OSHubHangar1Component::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseModuleComponent::VGenerateXML(root);

	return componentNode;
}