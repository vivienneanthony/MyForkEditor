#include "AlphaEngineStd.h"
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
bool OSHubCoreComponent::VDelegateInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("HubCoreData");
	if (node)
	{
		pugi::xml_node node = pData.child("Computers");
		m_ComputerCount = node.attribute("value").as_int();
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

pugi::xml_node OSHubCoreComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseModuleComponent::VGenerateXML(root);

	pugi::xml_node hubCoreDataNode = componentNode.append_child("HubCoreData");

	pugi::xml_node dataNode = hubCoreDataNode.append_child("Computers");
	pugi::xml_attribute attribute = dataNode.append_attribute("value");
	attribute.set_value(m_ComputerCount);

	return componentNode;
}