#include "EngineStd.h"
#include "OSReserveRoomComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSReserveRoomComponent::g_Name = String("OSReserveRoomComponent");

OSReserveRoomComponent::OSReserveRoomComponent(Context* context) : OSBaseRoomComponent(context)
{
	m_RoomType = ReserveRoom;
	m_IronCount = 0;
}

OSReserveRoomComponent::OSReserveRoomComponent() : OSBaseRoomComponent()
{
	m_RoomType = ReserveRoom;
	m_IronCount = 0;
}

OSReserveRoomComponent::~OSReserveRoomComponent ()
{

}

bool OSReserveRoomComponent::VDelegateInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("OSReserveRoomData");
	if (node)
	{
		pugi::xml_node childNode = node.child("Iron");
		m_IronCount = childNode.attribute("value").as_int();
	}
	return true;
}

void OSReserveRoomComponent::VPostInit(void)
{
	OSBaseRoomComponent::VPostInit();
}

void OSReserveRoomComponent::VUpdate(float timeStep)
{
	OSBaseRoomComponent::VUpdate(timeStep);
}

void OSReserveRoomComponent::VOnChanged(void)
{
	OSBaseRoomComponent::VOnChanged();

}

pugi::xml_node OSReserveRoomComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseRoomComponent::VGenerateXML(root);

	pugi::xml_node reserveRoomDataNode = componentNode.append_child("OSReserveRoomData");
	
	pugi::xml_node nodeData = reserveRoomDataNode.append_child("Iron");
	pugi::xml_attribute attribute = nodeData.append_attribute("value");
	attribute.set_value(m_IronCount);

	return componentNode;
}