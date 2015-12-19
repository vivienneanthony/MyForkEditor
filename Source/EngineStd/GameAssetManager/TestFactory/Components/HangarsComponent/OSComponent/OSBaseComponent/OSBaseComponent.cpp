#include "EngineStd.h"
#include "OSBaseComponent.h"

#include "../../OSModulesComponent/OSBaseModuleComponent/OSBaseModuleComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSBaseComponent::g_Name = String("OSBaseComponent");

unsigned int OSBaseComponent::m_ModulesCount = 0;


OSBaseComponent::OSBaseComponent (Context* context) : OSInterface(context)
{
	m_StationType = OSUNKNOWN;
	VInitializeAllDelegates();
}

OSBaseComponent::OSBaseComponent () : OSInterface()
{
	m_StationType = OSUNKNOWN;
	VInitializeAllDelegates();
}

OSBaseComponent::~OSBaseComponent ()
{
	VDestroyAllDelegates();
}

bool OSBaseComponent::VInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("Name");
	if (node)
	{
		String name = node.attribute("value").as_string();
		m_Name = name;
	}

	node = pData->child("OSModules");
	if (node)
	{
		// Loop through each child element and load the station component
		for (pugi::xml_node pNode = node.first_child(); pNode; pNode = pNode.next_sibling())
		{
			StrongNodePtr pBasePart = CreateStationModule(pNode);
			if (!pBasePart)
			{
				return false;
			}
			
			m_StationModules.Push(pBasePart);
		}
	}

	return VDelegateInit(pData);
}

void OSBaseComponent::VPostInit(void)
{
	Node* rootNode = GetNode();

	if (rootNode)
	{
		rootNode->SetName(m_Name);

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
		rootNode->SetName(m_Name);
	
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
	}
}

StrongNodePtr OSBaseComponent::CreateStationModule(pugi::xml_node& node)
{
	BaseGameLogic* pGameLogic = g_pApp->GetGameLogic();

	String resource = node.attribute("resource").as_string();


	StrongNodePtr pGameNode = pGameLogic->VCreateGameNode(resource, &node, NULL, INVALID_GAME_NODE_ID, false);
	if (pGameNode)
	{
		Event_Data_New_Game_Node pNewGameNodeEvent(pGameNode->GetID());
		VariantMap data = pNewGameNodeEvent.VSerialize();
		SendEvent(Event_Data_New_Game_Node::g_EventType, data);
	}
	else
	{
		URHO3D_LOGDEBUG("Failed to create Station Module from resources " + resource);
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

	m_StationType = OSType::OSUNKNOWN;
	m_ModulesCount = 0;
}

StationModules& OSBaseComponent::GetAllModules()
{
	return m_StationModules;
}

StrongNodePtr OSBaseComponent::GetModule(String name)
{
	Node* root = GetNode();
	if (root)
	{
		StrongNodePtr pModule = StrongNodePtr(root->GetChild(name, true));
		if (pModule)
		{
			return pModule;
		}
	}
	return StrongNodePtr();
}

StationModules& OSBaseComponent::GetModule(OSModuleType type)
{
	m_TempStationModules.Clear();

	// Call on each current created modules
	for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
	{
		OSBaseModuleComponent* pBaseModule = (*it)->GetComponent<OSBaseModuleComponent>();

		if (pBaseModule)
		{
			if (pBaseModule->GetModuleType() == type)
			{
				m_TempStationModules.Push(*it);
			}
		}
	}
	return m_TempStationModules;
}

int OSBaseComponent::GetStationModuleCount()
{
	return m_ModulesCount;
}

void OSBaseComponent::CreateModule(OSType& type)
{

}

void OSBaseComponent::CreateModule(pugi::xml_node& pData)
{
	Node* rootNode = GetNode();

	StrongNodePtr pBasePart = CreateStationModule(pData);
	m_StationModules.Push(pBasePart);

	if (rootNode && pBasePart)
	{
		rootNode->AddChild(pBasePart);
		m_ModulesCount++;

		URHO3D_LOGDEBUG("Station module with name - " + pBasePart->GetName() + " is added to OS with name - " + m_Name);
	}
}

void OSBaseComponent::DestroyModule(String name)
{
	
	// Call on each current created component
	for (StationModules::Iterator it = m_StationModules.Begin(); it != m_StationModules.End(); it++)
	{
		if ((*it)->GetName() == name)
		{
			it = m_StationModules.Erase(it);
			if (it == m_StationModules.End())
			{
				break;
			}
			m_ModulesCount--;
		}
	}
}

void OSBaseComponent::DestroyModule(OSModuleType type)
{

}

void OSBaseComponent::DestroyModule(StrongNodePtr pModule)
{
	m_StationModules.Erase(m_StationModules.Find(pModule));
	GetNode()->GetChild(pModule->GetName());
	m_ModulesCount--;
}