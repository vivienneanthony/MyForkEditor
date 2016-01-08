#include "AlphaEngineStd.h"
#include "OSBaseComponent.h"

#include "../../OSModuleComponent/OSBaseModuleComponent/OSBaseModuleComponent.h"
#include "../../OSDeckComponent/OSBaseDeckComponent/OSBaseDeckComponent.h"
#include "../../OSRoomComponent/OSBaseRoomComponent/OSBaseRoomComponent.h"


#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSBaseComponent::g_Name = String("OSBaseComponent");

unsigned int OSBaseComponent::m_ModulesCount = 0;
unsigned int OSBaseComponent::m_RoomsCount = 0;			
unsigned int OSBaseComponent::m_DecksCount = 0;

OSBaseComponent::OSBaseComponent (Context* context) : OSInterface(context)
{
	m_StationType = OSUNKNOWN;

	m_StationModules.Clear();
	m_TempStationModules.Clear();

	m_StationRooms.Clear();
	m_TempStationRooms.Clear();

	m_StationDecks.Clear();
	m_TempStationDecks.Clear();

	VInitializeAllDelegates();
}

OSBaseComponent::OSBaseComponent () : OSInterface()
{
	m_StationType = OSUNKNOWN;

	m_StationModules.Clear();
	m_TempStationModules.Clear();

	m_StationRooms.Clear();
	m_TempStationRooms.Clear();

	m_StationDecks.Clear();
	m_TempStationDecks.Clear();

	VInitializeAllDelegates();
}

OSBaseComponent::~OSBaseComponent ()
{
	m_StationModules.Clear();
	m_TempStationModules.Clear();
	m_ModulesCount = 0;

	m_StationRooms.Clear();
	m_TempStationRooms.Clear();
	m_RoomsCount = 0;

	m_StationDecks.Clear();
	m_TempStationDecks.Clear();
	m_DecksCount = 0;

	VDestroyAllDelegates();
}

bool OSBaseComponent::VInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("Name");
	if (node)
	{
		String name = node.attribute("value").as_string();
		m_Name = name;
	}

	node = pData.child("OSModules");
	if (node)
	{
		// Loop through each child element and load the station component
		for (pugi::xml_node pNode = node.first_child(); pNode; pNode = pNode.next_sibling())
		{
			StrongNodePtr pBasePart = CreateStationComponent(pNode);
			if (!pBasePart)
			{
				return false;
			}
			
			m_StationModules.Push(pBasePart);
		}
	}

	node = pData.child("OSRooms");
	if (node)
	{
		// Loop through each child element and load the station component
		for (pugi::xml_node pNode = node.first_child(); pNode; pNode = pNode.next_sibling())
		{
			StrongNodePtr pBasePart = CreateStationComponent(pNode);
			if (!pBasePart)
			{
				return false;
			}

			m_StationRooms.Push(pBasePart);
		}
	}

	node = pData.child("OSDecks");
	if (node)
	{
		// Loop through each child element and load the station component
		for (pugi::xml_node pNode = node.first_child(); pNode; pNode = pNode.next_sibling())
		{
			StrongNodePtr pBasePart = CreateStationComponent(pNode);
			if (!pBasePart)
			{
				return false;
			}

			m_StationDecks.Push(pBasePart);
		}
	}

	return VDelegateInit(pData);
}

void OSBaseComponent::VPostInit(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		// Call on each current created component
		for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;
				
				URHO3D_LOGDEBUG("Station module with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}
	
		// Call on each current created component
		for (StationRooms::Iterator it = m_StationRooms.Begin(); it != m_StationRooms.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;

				URHO3D_LOGDEBUG("Station room with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}

		// Call on each current created component
		for (StationDecks::Iterator it = m_StationDecks.Begin(); it != m_StationDecks.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;

				URHO3D_LOGDEBUG("Station deck with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}
	}
}

void OSBaseComponent::VUpdate(float timeStep)
{

}

void OSBaseComponent::VOnChanged(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		// Call on each current created component
		for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;

				URHO3D_LOGDEBUG("Station module with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}

		// Call on each current created component
		for (StationRooms::Iterator it = m_StationRooms.Begin(); it != m_StationRooms.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;

				URHO3D_LOGDEBUG("Station room with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}

		// Call on each current created component
		for (StationDecks::Iterator it = m_StationDecks.Begin(); it != m_StationDecks.End(); it++)
		{
			// We have to check, may be it has already added as child.
			// Then we avoid wrong component count and 
			// add again to rootNode as child.
			if (!(*it)->GetVar("Added").GetBool())
			{
				(*it)->SetVar("Added", true);
				rootNode->AddChild((*it));
				m_ModulesCount++;

				URHO3D_LOGDEBUG("Station deck with name - " + (*it)->GetName() + " is added to OS with name - " + m_Name);
			}
		}
	}
}

StrongNodePtr OSBaseComponent::CreateStationComponent(pugi::xml_node node)
{
	BaseGameLogic* pGameLogic = g_pApp->GetGameLogic();

	String resource = node.attribute("resource").as_string();

	StrongNodePtr pGameNode = pGameLogic->VCreateGameNode(resource, node, NULL, INVALID_GAME_NODE_ID, false);
	if (pGameNode)
	{
		Event_Data_New_Game_Node pNewGameNodeEvent(pGameNode->GetID());
		SendEvent(Event_Data_New_Game_Node::g_EventType, pNewGameNodeEvent.VSerialize());
	}
	else
	{
		URHO3D_LOGDEBUG("Failed to create Station Component from resources " + resource);
	}

	return pGameNode;
}


void OSBaseComponent::Upgrade(VariantMap& materials)
{

}

void OSBaseComponent::Destroy()
{
	m_StationModules.Clear();
	m_TempStationModules.Clear();
	m_ModulesCount = 0;

	m_StationRooms.Clear();
	m_TempStationRooms.Clear();
	m_RoomsCount = 0;

	m_StationDecks.Clear();
	m_TempStationDecks.Clear();
	m_DecksCount = 0;

	m_StationType = OSType::OSUNKNOWN;	
}

const StationModules& OSBaseComponent::GetModules(const OSModuleType& type)
{
	m_TempStationModules.Clear();

	// Call on each current created modules
	for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
	{
		SharedPtr<OSBaseModuleComponent> module = DynamicCast<OSBaseModuleComponent>((*it));
		if (module->GetModuleType() == type)
		{
			m_TempStationModules.Push(*it);
		}
	}
	return m_TempStationModules;
}

bool OSBaseComponent::CreateModule(pugi::xml_node pData)
{
	Node* rootNode = GetNode();

	StrongNodePtr pBasePart = CreateStationComponent(pData);
	if (rootNode && pBasePart)
	{
		pBasePart->SetVar("Added", true);
		rootNode->AddChild(pBasePart);

		m_StationModules.Push(pBasePart);
		m_ModulesCount++;
	
		URHO3D_LOGDEBUG("Station module with name - " + pBasePart->GetName() + " is added to OS with name - " + m_Name);
		return true;
	}

	URHO3D_LOGDEBUG("Station module with name - " + pBasePart->GetName() + " FAILED to add to OS with name - " + m_Name);
	return false;
}

void OSBaseComponent::DestroyModule(const GameNodeId& id)
{
	Node* pRootNode = GetNode();
	StrongNodePtr pModule = StrongNodePtr(pRootNode->GetChild(String(id)));
	
	if (pModule && pRootNode)
	{
		StationModules::Iterator it = m_StationModules.Find(pModule);
		if (it->NotNull())
		{
			m_StationModules.Erase(it);
			m_ModulesCount--;
		}
		else
		{
			URHO3D_LOGDEBUG("Station module with id - " + String(id) + " could not be found ");
		}
	}
}

void OSBaseComponent::DestroyModule(const OSModuleType& type)
{
	// Call on each current created modules
	StationModules::Iterator it = m_StationModules.Begin();
	while (it != m_StationModules.End())
	{
		SharedPtr<OSBaseModuleComponent> module = DynamicCast<OSBaseModuleComponent>((*it));
		if (module)
		{
			if (module->GetModuleType() == type)
			{
				Node* pNode = GetNode();
				if (pNode)
				{
					pNode->RemoveChild((*it));
					it = m_StationModules.Erase(m_StationModules.Find(*it));
					m_ModulesCount--;
					URHO3D_LOGDEBUG("Station module " + (*it)->GetVar("type").ToString() + " with id " + (*it)->GetName() + " successfuly deleted from station " + m_Name);
				}
			}
			else
			{
				++it;
			}
		}
	}
}

void OSBaseComponent::DestroyModule(StrongNodePtr pModule)
{
	Node* pNode = GetNode();
	if (pNode)
	{
		m_StationModules.Remove(pModule);
		pNode->RemoveChild(pModule);
		m_ModulesCount--;
		URHO3D_LOGDEBUG("Station module " + pModule->GetVar("type").ToString() + " with id " + pModule->GetName() + " successfuly deleted from station " + m_Name);
	}
}

const StationRooms& OSBaseComponent::GetRooms(const OSRoomType& type)
{
	m_TempStationRooms.Clear();

	// Call on each current created rooms
	for (StationRooms::Iterator it = m_StationRooms.Begin(); it != m_StationRooms.End(); it++)
	{
		SharedPtr<OSBaseRoomComponent> room = DynamicCast<OSBaseRoomComponent>((*it));
		if (room->GetRoomType() == type)
		{
			m_TempStationModules.Push(*it);
		}
	}
	return m_TempStationRooms;
}

bool OSBaseComponent::CreateRoom(pugi::xml_node pData)
{
	Node* rootNode = GetNode();

	StrongNodePtr pBasePart = CreateStationComponent(pData);
	if (rootNode && pBasePart)
	{
		pBasePart->SetVar("Added", true);
		rootNode->AddChild(pBasePart);

		m_StationRooms.Push(pBasePart);
		m_RoomsCount++;

		URHO3D_LOGDEBUG("Station room with name - " + pBasePart->GetName() + " is added to OS with name - " + m_Name);
		return true;
	}

	URHO3D_LOGDEBUG("Station room with name - " + pBasePart->GetName() + " FAILED to add to OS with name - " + m_Name);
	return false;
}

void OSBaseComponent::DestroyRoom(const GameNodeId& id)
{
	Node* pRootNode = GetNode();
	StrongNodePtr pRoom = StrongNodePtr(pRootNode->GetChild(String(id)));

	if (pRoom && pRootNode)
	{
		StationRooms::Iterator it = m_StationRooms.Find(pRoom);
		if (it->NotNull())
		{
			m_StationRooms.Erase(it);
			m_RoomsCount--;
		}
		else
		{
			URHO3D_LOGDEBUG("Station room with id - " + String(id) + " could not be found ");
		}
	}
}

void OSBaseComponent::DestroyRoom(const OSRoomType& type)
{
	// Call on each current created room
	StationRooms::Iterator it = m_StationRooms.Begin();
	while (it != m_StationRooms.End())
	{
		SharedPtr<OSBaseRoomComponent> room = DynamicCast<OSBaseRoomComponent>((*it));
		if (room)
		{
			if (room->GetRoomType() == type)
			{
				Node* pNode = GetNode();
				if (pNode)
				{
					pNode->RemoveChild((*it));
					it = m_StationRooms.Erase(m_StationRooms.Find(*it));
					m_RoomsCount--;
					URHO3D_LOGDEBUG("Station room " + (*it)->GetVar("type").ToString() + " with id " + (*it)->GetName() + " successfuly deleted from station " + m_Name);
				}
			}
			else
			{
				++it;
			}
		}
	}
}

void OSBaseComponent::DestroyRoom(StrongNodePtr pRoom)
{
	Node* pNode = GetNode();
	if (pNode)
	{
		m_StationRooms.Remove(pRoom);
		pNode->RemoveChild(pRoom);
		m_RoomsCount--;
		URHO3D_LOGDEBUG("Station room " + pRoom->GetVar("type").ToString() + " with id " + pRoom->GetName() + " successfuly deleted from station " + m_Name);
	}
}

const StationDecks& OSBaseComponent::GetDecks(const OSDeckType& type)
{
	m_TempStationDecks.Clear();

	// Call on each current created decks
	for (StationDecks::Iterator it = m_StationDecks.Begin(); it != m_StationDecks.End(); it++)
	{
		SharedPtr<OSBaseDeckComponent> deck = DynamicCast<OSBaseDeckComponent>((*it));
		if (deck->GetDeckType() == type)
		{
			m_TempStationDecks.Push(*it);
		}
	}
	return m_TempStationDecks;
}

bool OSBaseComponent::CreateDeck(pugi::xml_node pData)
{
	Node* rootNode = GetNode();

	StrongNodePtr pBasePart = CreateStationComponent(pData);
	if (rootNode && pBasePart)
	{
		pBasePart->SetVar("Added", true);
		rootNode->AddChild(pBasePart);

		m_StationDecks.Push(pBasePart);
		m_DecksCount++;

		URHO3D_LOGDEBUG("Station deck with name - " + pBasePart->GetName() + " is added to OS with name - " + m_Name);
		return true;
	}

	URHO3D_LOGDEBUG("Station deck with name - " + pBasePart->GetName() + " FAILED to add to OS with name - " + m_Name);
	return false;
}

void OSBaseComponent::DestroyDeck(const GameNodeId& id)
{
	Node* pRootNode = GetNode();
	StrongNodePtr pDeck = StrongNodePtr(pRootNode->GetChild(String(id)));

	if (pDeck && pRootNode)
	{
		StationDecks::Iterator it = m_StationDecks.Find(pDeck);
		if (it->NotNull())
		{
			m_StationDecks.Erase(it);
			m_DecksCount--;
		}
		else
		{
			URHO3D_LOGDEBUG("Station deck with id - " + String(id) + " could not be found ");
		}
	}
}

void OSBaseComponent::DestroyDeck(const OSDeckType& type)
{
	// Call on each current created deck
	StationDecks::Iterator it = m_StationDecks.Begin();
	while (it != m_StationDecks.End())
	{
		SharedPtr<OSBaseDeckComponent> deck = DynamicCast<OSBaseDeckComponent>((*it));
		if (deck)
		{
			if (deck->GetDeckType() == type)
			{
				Node* pNode = GetNode();
				if (pNode)
				{
					pNode->RemoveChild((*it));
					it = m_StationDecks.Erase(m_StationDecks.Find(*it));
					m_DecksCount--;
					URHO3D_LOGDEBUG("Station deck " + (*it)->GetVar("type").ToString() + " with id " + (*it)->GetName() + " successfuly deleted from station " + m_Name);
				}
			}
			else
			{
				++it;
			}
		}
	}
}

void OSBaseComponent::DestroyDeck(StrongNodePtr pDeck)
{
	Node* pNode = GetNode();
	if (pNode)
	{
		m_StationDecks.Remove(pDeck);
		pNode->RemoveChild(pDeck);
		m_DecksCount--;
		URHO3D_LOGDEBUG("Station deck " + pDeck->GetVar("type").ToString() + " with id " + pDeck->GetName() + " successfuly deleted from station " + m_Name);
	}
}


pugi::xml_node OSBaseComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node childNode = componentNode.append_child("Name");
	pugi::xml_attribute attribute = childNode.append_attribute("value");
	attribute.set_value(m_Name.CString());

	pugi::xml_node modulesNode = componentNode.append_child("OSModules");

	for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
	{
		OSBaseModuleComponent::VGenerateXML((*it), modulesNode);
	}

	pugi::xml_node roomsNode = componentNode.append_child("OSRooms");
	for (StationRooms::Iterator it = m_StationRooms.Begin(); it != m_StationRooms.End(); it++)
	{
		OSBaseRoomComponent::VGenerateXML((*it), roomsNode);
	}

	pugi::xml_node decksNode = componentNode.append_child("OSDecks");
	for (StationDecks::Iterator it = m_StationDecks.Begin(); it != m_StationDecks.End(); it++)
	{
		OSBaseDeckComponent::VGenerateXML((*it), decksNode);
	}

	return componentNode;
}
