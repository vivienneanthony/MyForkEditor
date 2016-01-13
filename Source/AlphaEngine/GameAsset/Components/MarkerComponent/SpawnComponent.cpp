#include "AlphaEngineStd.h"
#include "SpawnComponent.h"

const String SpawnComponent::g_Name = "SpawnComponent";

SpawnComponent::SpawnComponent(Context* context) : MainComponent(context)
{

}

SpawnComponent::SpawnComponent() : MainComponent()
{

}

SpawnComponent::~SpawnComponent()
{

}

// mainComponent interface
bool SpawnComponent::VInit(pugi::xml_node pData)
{

	return true;
}

void SpawnComponent::VPostInit(void)
{
}

void SpawnComponent::VUpdate(float timeStep)
{

}

void SpawnComponent::VOnChanged(void)
{
}


pugi::xml_node SpawnComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	return componentNode;
}
