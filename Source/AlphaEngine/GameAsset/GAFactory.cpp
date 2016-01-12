#include "AlphaEngineStd.h"

#include "Resources/XMLResourceLoader/XMLResourceLoader.h"

// Main component
#include "Components/MainComponent.h"

// Transform
#include "Components/TransformComponent/TransformComponent.h"

// Render
#include "Components/RenderComponent/LightComponent/BaseLightComponent.h"
#include "Components/RenderComponent/ModelComponent/StaticModelComponent.h"
#include "Components/RenderComponent/SkyboxComponent/SkyBoxComponent.h"

// Station
#include "Components/HangarsComponent/OSComponent/OSBaseComponent/OSBaseComponent.h"
#include "Components/HangarsComponent/OSComponent/OSOrcasComponent/OSOrcasComponent.h"

// Station Modules
#include "Components/HangarsComponent/OSModuleComponent/OSBaseModuleComponent/OSBaseModuleComponent.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubCoreComponent/OSHubCoreComponent.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubCommArray2Component/OSHubCommArray2Component.h"
#include "Components/HangarsComponent/OSModuleComponent/OSTrussTriple1Component/OSTrussTriple1Component.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubHangar2Component/OSHubHangar2Component.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubCoreComponent/OSHubCoreComponent.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubHangar2Component/OSHubHangar2Component.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHubHangar1Component/OSHubHangar1Component.h"
#include "Components/HangarsComponent/OSModuleComponent/OSHub1Component/OSHub1Component.h"

// Station Deck
#include "Components/HangarsComponent/OSDeckComponent/OSBaseDeckComponent/OSBaseDeckComponent.h"
#include "Components/HangarsComponent/OSDeckComponent/OSSpaceShipDeckComponent/OSSpaceShipDeckComponent.h"

// Station rooms
#include "Components/HangarsComponent/OSRoomComponent/OSBaseRoomComponent/OSBaseRoomComponent.h"
#include "Components/HangarsComponent/OSRoomComponent/OSReserveRoomComponent/OSReserveRoomComponent.h"

// Chemistry
#include "Components/ChemistryComponent/RawMaterialComponent/RawMaterialComponent.h"
#include "Components/ChemistryComponent/ChemicalComponent/ChemicalComponent.h"

#include "GAFactory.h"

GAFactory::GAFactory(Context* context) : Object(context)
{
	m_LastGameNodeId = 10;

	// Render components
	m_ComponentFactory.Register<TransformComponent>(GetUniques::GetIdFromName(TransformComponent::g_Name));
	m_ComponentFactory.Register<BaseLightComponent>(GetUniques::GetIdFromName(BaseLightComponent::g_Name));
	m_ComponentFactory.Register<StaticModelComponent>(GetUniques::GetIdFromName(StaticModelComponent::g_Name));
	m_ComponentFactory.Register<SkyBoxComponent>(GetUniques::GetIdFromName(SkyBoxComponent::g_Name));

	// Station
	m_ComponentFactory.Register<OSBaseComponent>(GetUniques::GetIdFromName(OSBaseComponent::g_Name));
	m_ComponentFactory.Register<OSOrcasComponent>(GetUniques::GetIdFromName(OSOrcasComponent::g_Name));

	// Station modules
	m_ComponentFactory.Register<OSBaseModuleComponent>(GetUniques::GetIdFromName(OSBaseModuleComponent::g_Name));
	m_ComponentFactory.Register<OSHub1Component>(GetUniques::GetIdFromName(OSHub1Component::g_Name));
	m_ComponentFactory.Register<OSHubCoreComponent>(GetUniques::GetIdFromName(OSHubCoreComponent::g_Name));
	m_ComponentFactory.Register<OSHubCommArray2Component>(GetUniques::GetIdFromName(OSHubCommArray2Component::g_Name));
	m_ComponentFactory.Register<OSTrussTriple1Component>(GetUniques::GetIdFromName(OSTrussTriple1Component::g_Name));
	m_ComponentFactory.Register<OSHubHangar2Component>(GetUniques::GetIdFromName(OSHubHangar2Component::g_Name));
	m_ComponentFactory.Register<OSHubHangar1Component>(GetUniques::GetIdFromName(OSHubHangar1Component::g_Name));
	m_ComponentFactory.Register<OSHub1Component>(GetUniques::GetIdFromName(OSHub1Component::g_Name));

	// Station Deck
	m_ComponentFactory.Register<OSBaseDeckComponent>(GetUniques::GetIdFromName(OSBaseDeckComponent::g_Name));
	m_ComponentFactory.Register<OSSpaceShipDeckComponent>(GetUniques::GetIdFromName(OSSpaceShipDeckComponent::g_Name));

	// Station rooms
	m_ComponentFactory.Register<OSBaseRoomComponent>(GetUniques::GetIdFromName(OSBaseRoomComponent::g_Name));
	m_ComponentFactory.Register<OSReserveRoomComponent>(GetUniques::GetIdFromName(OSReserveRoomComponent::g_Name));

	// Chemistry
	m_ComponentFactory.Register<RawMaterialComponent>(GetUniques::GetIdFromName(RawMaterialComponent::g_Name));
	m_ComponentFactory.Register<ChemicalComponent>(GetUniques::GetIdFromName(ChemicalComponent::g_Name));

	// Basic components
	context_->RegisterFactory<MainComponent>();
	context_->RegisterFactory<TransformComponent>();

	// Render components
	context_->RegisterFactory<BaseLightComponent>();
	context_->RegisterFactory<StaticModelComponent>();
	context_->RegisterFactory<SkyBoxComponent>();

	// Station components
	context_->RegisterFactory<OSBaseComponent>();
	context_->RegisterFactory<OSHubCoreComponent>();
	context_->RegisterFactory<OSHubCommArray2Component>();
	context_->RegisterFactory<OSTrussTriple1Component>();
	context_->RegisterFactory<OSHubHangar2Component>();
	context_->RegisterFactory<OSHubHangar1Component>();
	context_->RegisterFactory<OSHub1Component>();

	// Station Deck
	context_->RegisterFactory<OSBaseDeckComponent>();
	context_->RegisterFactory<OSSpaceShipDeckComponent>();

	// Station rooms
	context_->RegisterFactory<OSBaseRoomComponent>();
	context_->RegisterFactory<OSReserveRoomComponent>();

	// Chemistry
	context_->RegisterFactory<RawMaterialComponent>();
	context_->RegisterFactory<ChemicalComponent>();

}

GAFactory::~GAFactory()
{

}


StrongNodePtr GAFactory::CreateNode(String resource, pugi::xml_node overrides, const Matrix4* initTransform, const GameNodeId serversId)
{
	//assert(m_Components.Empty());

	pugi::xml_document* document = XMLResourceLoader::LoadXMLDocument(resource);
	pugi::xml_node root = document->first_child();

	if (!document && !root)
	{
		return StrongNodePtr();
	}

	GameNodeId gameNodeId;

	if (serversId == INVALID_GAME_NODE_ID)
	{
		gameNodeId = GetNextGameNodeId();
	}
	else
	{
		gameNodeId = serversId;
	}

	StrongNodePtr pGameNode(new Node(context_));
	pGameNode->SetID(gameNodeId);

	if (!InitNode(pGameNode, root))
	{
		URHO3D_LOGERROR("Failed to initialize game node from resource" + resource);
		return StrongNodePtr();
	}

	// Loop through each child element and load the component
	for (pugi::xml_node pNode = root.first_child(); pNode; pNode = pNode.next_sibling())
	{
		StrongGameNodeComponentPtr pComponent(VCreateComponent(pNode));
		if (pComponent)
		{
			pGameNode->AddComponent(pComponent, GetUniques::GetUniqueIdForEngine(pGameNode->GetID(), pComponent->VGetName()), pComponent->GetCreateMode());
			m_Components.Push(pComponent);
		}
		else
		{
			m_Components.Clear();
			return StrongNodePtr();
		}
	}

	if (overrides)
	{
		ModifyGameNode(pGameNode, overrides);
	}

	// Call on each current created component
	for (GameNodeComponents::Iterator it = m_Components.Begin(); it != m_Components.End(); it++)
	{
		(*it)->VPostInit();
	}
	m_Components.Clear();

	//SAFE_DELETE(document);

	return pGameNode;
}

// Modify node
void GAFactory::ModifyGameNode(StrongNodePtr node, pugi::xml_node overrides, bool calledFromCreateNode)
{
	// Loop through each child element and load the component
	for (pugi::xml_node pNode = overrides.first_child(); pNode; pNode = pNode.next_sibling())
	{

		String name = pNode.attribute("name").as_string();

		WeakGameNodeComponentPtr pComponent = StrongGameNodeComponentPtr((MainComponent*)node->GetComponent(name, true));
		if (pComponent.NotNull())
		{
			pComponent->VInit(pNode);
			// Ensure that components that need it have
			// Events generated that can notify subsystems when changes happen.
			// This was done to have SceneNode derived classes respond to RenderComponent
			// changes.
			pComponent->VOnChanged();
		}
		else
		{
			pComponent = VCreateComponent(pNode);
			if (pComponent)
			{
				node->AddComponent(MakeStrongPtr(pComponent), GetUniques::GetUniqueIdForEngine(node->GetID(), pComponent->VGetName()), pComponent->GetCreateMode());
			}

			m_Components.Push(MakeStrongPtr(pComponent));
		}
	}

	if (!calledFromCreateNode)
	{
		for (GameNodeComponents::Iterator it = m_Components.Begin();; it++)
		{
			(*it)->VPostInit();
		}
		m_Components.Clear();
	}
}


bool GAFactory::InitNode(StrongNodePtr node, pugi::xml_node pData)
{
	if (pData)
	{
		String gameNodeType = pData.attribute("type").as_string();
		String resource = pData.attribute("resource").as_string();
		GameNodeId id = node->GetID();
		// Convert id to name
		node->SetName(String(id));

		node->SetVar("type", gameNodeType);
		node->SetVar("resource", resource);

		URHO3D_LOGDEBUG("Initializing GameNode " + gameNodeType + " with id = " + String(node->GetID()));

		return true;
	}



	return false;
}

StrongGameNodeComponentPtr GAFactory::VCreateComponent(pugi::xml_node pData)
{
	String name = pData.attribute("name").as_string();
	StrongGameNodeComponentPtr pComponent(m_ComponentFactory.Create(GetUniques::GetIdFromName(name)));

	if (pComponent)
	{
		if (!pComponent->VInit(pData))
		{
			URHO3D_LOGERROR("Component failed to initialize: " + name);
			return StrongGameNodeComponentPtr();
		}
	}
	else
	{
		URHO3D_LOGERROR("Couldn't find GameNodeComponent named " + name);
		return StrongGameNodeComponentPtr();
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a
	// custom CreateComponent() function in a sub class.
	return pComponent;
}

void GAFactory::ToXML(pugi::xml_document& document, StrongNodePtr node)
{
	// GameNode element
	pugi::xml_node nodeXML = document.append_child("GameAsset");

	pugi::xml_attribute nodeAttribute = nodeXML.append_attribute("type");
	nodeAttribute.set_value(node->GetVar("type").ToString().CString());

	nodeAttribute = nodeXML.append_attribute("resource");
	nodeAttribute.set_value(node->GetVar("resource").ToString().CString());

	const Vector<SharedPtr<Component>>& components = node->GetComponents();
	Vector<SharedPtr<Component>>::ConstIterator it = components.Begin();

	for (; it != components.End(); it++)
	{
		StrongGameNodeComponentPtr component = DynamicCast<MainComponent>((*it));
		if (component)
		{
			component->VGenerateXML(nodeXML);
		}
	}
}
