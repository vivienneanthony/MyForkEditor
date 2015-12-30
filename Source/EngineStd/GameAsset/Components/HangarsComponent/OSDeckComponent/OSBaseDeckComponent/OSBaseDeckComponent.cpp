#include "EngineStd.h"
#include "OSBaseDeckComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSBaseDeckComponent::g_Name = String("OSBaseDeckComponent");

OSBaseDeckComponent::OSBaseDeckComponent(Context* context) : OSDeckInterface(context)
{
	m_DeckType = DeckUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseDeckComponent::OSBaseDeckComponent() : OSDeckInterface()
{
	m_DeckType = DeckUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseDeckComponent::~OSBaseDeckComponent ()
{

}

bool OSBaseDeckComponent::VInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("Name");
	if (node)
	{
		String name = node.attribute("value").as_string();
		m_Name = name;
	}

	return VDelegateInit(pData);
}

void OSBaseDeckComponent::VPostInit(void)
{
	Node* rootNode = GetNode();
	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}

}

void OSBaseDeckComponent::VUpdate(float timeStep)
{
	

}

void OSBaseDeckComponent::VOnChanged(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}
}

pugi::xml_node OSBaseDeckComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node childNode = componentNode.append_child("Name");
	pugi::xml_attribute attribute = childNode.append_attribute("value");
	attribute.set_value(m_Name.CString());
	
	return componentNode;
}

pugi::xml_node OSBaseDeckComponent::VGenerateXML(StrongNodePtr deckNode, pugi::xml_node root)
{
	pugi::xml_node nodeXML = root.append_child("GameAsset");

	pugi::xml_attribute nodeAttribute = nodeXML.append_attribute("type");
	nodeAttribute.set_value(deckNode->GetVar("type").ToString().CString());

	nodeAttribute = nodeXML.append_attribute("resource");
	nodeAttribute.set_value(deckNode->GetVar("resource").ToString().CString());

	const Vector<SharedPtr<Component>>& components = deckNode->GetComponents();
	Vector<SharedPtr<Component>>::ConstIterator it = components.Begin();

	for (; it != components.End(); it++)
	{
		StrongGameNodeComponentPtr component = DynamicCast<MainComponent>((*it));
		if (component)
		{
			component->VGenerateXML(nodeXML);
		}
	}

	return nodeXML;
}

