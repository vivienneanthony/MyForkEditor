#include "EngineStd.h"
#include "OSBaseRoomComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSBaseRoomComponent::g_Name = String("OSBaseRoomComponent");

OSBaseRoomComponent::OSBaseRoomComponent(Context* context) : OSRoomInterface(context)
{
	m_RoomType = RoomUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseRoomComponent::OSBaseRoomComponent() : OSRoomInterface()
{
	m_RoomType = RoomUNKNOWN;
	m_Name = String::EMPTY;
}

OSBaseRoomComponent::~OSBaseRoomComponent ()
{

}

bool OSBaseRoomComponent::VInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("Name");
	if (node)
	{
		String name = node.attribute("value").as_string();
		m_Name = name;
	}

	return VDelegateInit(pData);
}

void OSBaseRoomComponent::VPostInit(void)
{
	Node* rootNode = GetNode();
	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}

}

void OSBaseRoomComponent::VUpdate(float timeStep)
{
	

}

void OSBaseRoomComponent::VOnChanged(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		rootNode->SetName(m_Name);
	}
}

pugi::xml_node OSBaseRoomComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node nodeChild = componentNode.append_child("Name");
	pugi::xml_attribute attribute = nodeChild.append_attribute("value");
	attribute.set_value(m_Name.CString());

	return componentNode;
}

pugi::xml_node OSBaseRoomComponent::VGenerateXML(StrongNodePtr roomNode, pugi::xml_node root)
{
	pugi::xml_node nodeXML = root.append_child("GameAsset");

	pugi::xml_attribute nodeAttribute = nodeXML.append_attribute("type");
	nodeAttribute.set_value(roomNode->GetVar("type").ToString().CString());

	nodeAttribute = nodeXML.append_attribute("resource");
	nodeAttribute.set_value(roomNode->GetVar("resource").ToString().CString());

	const Vector<SharedPtr<Component>>& components = roomNode->GetComponents();
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