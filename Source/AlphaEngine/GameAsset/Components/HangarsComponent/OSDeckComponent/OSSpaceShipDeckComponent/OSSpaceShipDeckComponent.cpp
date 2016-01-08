#include "AlphaEngineStd.h"
#include "OSSpaceShipDeckComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSSpaceShipDeckComponent::g_Name = String("OSSpaceShipDeckComponent");

OSSpaceShipDeckComponent::OSSpaceShipDeckComponent(Context* context) : OSBaseDeckComponent(context)
{
	m_DeckType = SpaceShipDeck;
	m_SpaceShipCount = 1;
}

OSSpaceShipDeckComponent::OSSpaceShipDeckComponent() : OSBaseDeckComponent()
{
	m_DeckType = SpaceShipDeck;
	m_SpaceShipCount = 1;
}

OSSpaceShipDeckComponent::~OSSpaceShipDeckComponent ()
{

}

bool OSSpaceShipDeckComponent::VDelegateInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("OSSpaceShipDeckData");
	if (node)
	{
		pugi::xml_node childNode = node.child("SpaceShips");
		m_SpaceShipCount = childNode.attribute("value").as_int();
	}
	return true;
}

void OSSpaceShipDeckComponent::VPostInit(void)
{
	OSBaseDeckComponent::VPostInit();

}

void OSSpaceShipDeckComponent::VUpdate(float timeStep)
{
	OSBaseDeckComponent::VUpdate(timeStep);

}

void OSSpaceShipDeckComponent::VOnChanged(void)
{
	OSBaseDeckComponent::VOnChanged();
}

pugi::xml_node OSSpaceShipDeckComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseDeckComponent::VGenerateXML(root);

	pugi::xml_node spaceShipDeckDataNode = componentNode.append_child("OSSpaceShipDeckData");

	pugi::xml_node dataNode = spaceShipDeckDataNode.append_child("SpaceShips");
	pugi::xml_attribute attribute = dataNode.append_attribute("value");
	attribute.set_value(m_SpaceShipCount);

	return componentNode;
}