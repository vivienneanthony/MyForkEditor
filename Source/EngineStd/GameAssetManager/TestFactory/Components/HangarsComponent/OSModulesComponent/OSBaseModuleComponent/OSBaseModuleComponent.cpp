#include "EngineStd.h"
#include "OSBaseModuleComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSBaseModuleComponent::g_Name = String("OSBaseModuleComponent");

OSBaseModuleComponent::OSBaseModuleComponent(Context* context) : OSModuleInterface(context)
{
	m_ModuleType = ModuleUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseModuleComponent::OSBaseModuleComponent() : OSModuleInterface()
{
	m_ModuleType = ModuleUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseModuleComponent::~OSBaseModuleComponent ()
{

}

bool OSBaseModuleComponent::VInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("Name");
	if (node)
	{
		String name = node.attribute("value").as_string();
		m_Name = name;
	}

	return VDelegateInit(pData);
}

void OSBaseModuleComponent::VPostInit(void)
{
	Node* rootNode = GetNode();
	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}

}

void OSBaseModuleComponent::VUpdate(float timeStep)
{
	

}

void OSBaseModuleComponent::VOnChanged(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}
}

