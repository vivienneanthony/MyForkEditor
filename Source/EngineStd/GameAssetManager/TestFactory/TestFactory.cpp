#include "EngineStd.h"

#include "Resources/XMLResourceLoader/XMLResourceLoader.h"

#include "Components/MainComponent.h"
#include "Components/TransformComponent/TransformComponent.h"

#include "Components/RenderComponent/LightComponent/BaseLightComponent.h"
#include "Components/RenderComponent/ModelComponent/StaticModelComponent.h"
#include "Components/RenderComponent/SkyBoxComponent/SkyBoxComponent.h"

#include "Components/HangarsComponent/OSComponent/OSBaseComponent/OSBaseComponent.h"
#include "Components/HangarsComponent/OSComponent/OSOrcasComponent/OSOrcasComponent.h"


#include "Components/HangarsComponent/OSModulesComponent/OSBaseModuleComponent/OSBaseModuleComponent.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubCoreComponent/OSHubCoreComponent.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubCommArray2Component/OSHubCommArray2Component.h"
#include "Components/HangarsComponent/OSModulesComponent/OSTrussTriple1Component/OSTrussTriple1Component.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubHangar2Component/OSHubHangar2Component.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubCoreComponent/OSHubCoreComponent.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubHangar2Component/OSHubHangar2Component.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHubHangar1Component/OSHubHangar1Component.h"
#include "Components/HangarsComponent/OSModulesComponent/OSHub1Component/OSHub1Component.h"

#include "TestFactory.h"

TestFactory::TestFactory(Context* context) : Object(context)
{
	m_LastGameNodeId = 10;

	// Basic components
	m_ComponentFactory.Register<TransformComponent>(MainComponent::GetIdFromName(TransformComponent::g_Name));
	m_ComponentFactory.Register<BaseLightComponent>(MainComponent::GetIdFromName(BaseLightComponent::g_Name));
	m_ComponentFactory.Register<StaticModelComponent>(MainComponent::GetIdFromName(StaticModelComponent::g_Name));
	m_ComponentFactory.Register<SkyBoxComponent>(MainComponent::GetIdFromName(SkyBoxComponent::g_Name));

	// Station
	m_ComponentFactory.Register<OSBaseComponent>(MainComponent::GetIdFromName(OSBaseComponent::g_Name));
	m_ComponentFactory.Register<OSOrcasComponent>(MainComponent::GetIdFromName(OSOrcasComponent::g_Name));

	// Station modules
	m_ComponentFactory.Register<OSBaseModuleComponent>(MainComponent::GetIdFromName(OSBaseModuleComponent::g_Name));
	m_ComponentFactory.Register<OSHub1Component>(MainComponent::GetIdFromName(OSHub1Component::g_Name));
	m_ComponentFactory.Register<OSHubCoreComponent>(MainComponent::GetIdFromName(OSHubCoreComponent::g_Name));
	m_ComponentFactory.Register<OSHubCommArray2Component>(MainComponent::GetIdFromName(OSHubCommArray2Component::g_Name));
	m_ComponentFactory.Register<OSTrussTriple1Component>(MainComponent::GetIdFromName(OSTrussTriple1Component::g_Name));
	m_ComponentFactory.Register<OSHubHangar2Component>(MainComponent::GetIdFromName(OSHubHangar2Component::g_Name));
	m_ComponentFactory.Register<OSHubHangar1Component>(MainComponent::GetIdFromName(OSHubHangar1Component::g_Name));
	m_ComponentFactory.Register<OSHub1Component>(MainComponent::GetIdFromName(OSHub1Component::g_Name));

	// Basic components
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
}

TestFactory::~TestFactory()
{

}


StrongNodePtr TestFactory::CreateNode(String resource, pugi::xml_node* overrides, const Matrix4* initTransform, const GameNodeId serversId)
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

	if (!InitNode(pGameNode, &root))
	{
		URHO3D_LOGERROR("Failed to initialize game node from resource" + resource);
		return StrongNodePtr();
	}

	// Loop through each child element and load the component
	for (pugi::xml_node pNode = root.first_child(); pNode; pNode = pNode.next_sibling())
	{
		StrongGameNodeComponentPtr pComponent(VCreateComponent(&pNode));
		if (pComponent)
		{
			pGameNode->AddComponent(pComponent, MainComponent::GetUniqueIdForEngine(pGameNode->GetID(), pComponent->VGetName()), pComponent->GetCreateMode());
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
void TestFactory::ModifyGameNode(StrongNodePtr node, pugi::xml_node* overrides, bool calledFromCreateNode)
{
	// Loop through each child element and load the component
	for (pugi::xml_node pNode = overrides->first_child(); pNode; pNode = pNode.next_sibling())
	{

		String name = pNode.attribute("name").as_string();
		
		WeakGameNodeComponentPtr pComponent = StrongGameNodeComponentPtr((MainComponent*)node->GetComponent(name, true));
		if (pComponent.NotNull())
		{
			pComponent->VInit(&pNode);
			// Ensure that components that need it have
			// Events generated that can notify subsystems when changes happen.
			// This was done to have SceneNode derived classes respond to RenderComponent
			// changes.
			pComponent->VOnChanged();
		}
		else
		{
			pComponent = VCreateComponent(&pNode);
			if (pComponent)
			{
				node->AddComponent(MakeStrongPtr(pComponent), MainComponent::GetUniqueIdForEngine(node->GetID(), pComponent->VGetName()), pComponent->GetCreateMode());
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


bool TestFactory::InitNode(StrongNodePtr node, pugi::xml_node* pData)
{

	URHO3D_LOGDEBUG("Initializing GameNode with id = " + String(node->GetID()));

	if (pData)
	{
		String m_GameNodeType = pData->attribute("type").as_string();
		String m_Resource = pData->attribute("resource").as_string();

		node->SetName(m_GameNodeType);
		node->SetVar("resource", m_Resource);

		return true;
	}

	return false;
}

StrongGameNodeComponentPtr TestFactory::VCreateComponent(pugi::xml_node* pData)
{
	String name = pData->attribute("name").as_string();
	StrongGameNodeComponentPtr pComponent(m_ComponentFactory.Create(MainComponent::GetIdFromName(name)));

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