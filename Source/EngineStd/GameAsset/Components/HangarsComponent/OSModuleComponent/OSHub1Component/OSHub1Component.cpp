#include "EngineStd.h"
#include "OSHub1Component.h"

const String OSHub1Component::g_Name = String("OSHub1Component");

OSHub1Component::OSHub1Component(Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = Hub1;
}

OSHub1Component::OSHub1Component() : OSBaseModuleComponent()
{
	m_ModuleType = Hub1;
}

OSHub1Component::~OSHub1Component()
{

}

// mainComponent interface
bool OSHub1Component::VDelegateInit(pugi::xml_node pData)
{

	return true;
}

void OSHub1Component::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSHub1Component::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSHub1Component::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}

pugi::xml_node OSHub1Component::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseModuleComponent::VGenerateXML(root);
	return componentNode;
}
