#include "EngineStd.h"

// Pickup components
#include "Components/Pickup/Ammo/AmmoComponent.h"
#include "Components/Pickup/Health/HealthComponent.h"

#include "GameAssetFactory.h"


GameAssetFactory::GameAssetFactory(Context* context_) : Object(context_)
{
	m_LastGameNodeId = INVALID_GAME_NODE_ID;

	// There should be registered game asset components
	m_ComponentFactory.Register<AmmoPickup>((unsigned int)AmmoPickup::g_Type);
	m_ComponentFactory.Register<HealthPickup>((unsigned int)HealthPickup::g_Type);

}

GameAssetFactory::~GameAssetFactory()
{

}

StrongNodePtr GameAssetFactory::CreateNode(GameAsset* gameAsset, GameNodeId serversId)
{
	GameNodeId nextGameNodeId = serversId;
	if (nextGameNodeId == INVALID_GAME_NODE_ID)
	{
		nextGameNodeId = GetNextGameNodeId();
	}

	StrongNodePtr pGameNode(new Node(context_));

	pGameNode->SetID(nextGameNodeId);

	// Create root component
	// Loop through each game asset child element and load the component
	BaseComponent* component = VCreateComponent(gameAsset);
	if (component)
	{
		// *ITISSCAN* 23.11.2015. 
		// Not to good cast from GameAssetType structure to unsigned int...
		// Maybe in future better to make StringHash instead?
		pGameNode->AddComponent(component, component->GetID(), component->GetCreateMode());
	}
	else
	{
		// If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be 
		// partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
		// will fall out of scope with nothing else pointing to it.
		return StrongNodePtr();
	}


	// Create childs components
	Vector<GameAsset*> childs = gameAsset->GetChilds();
	Vector<GameAsset*>::Iterator it = childs.Begin();
	for (; it != childs.End(); it++)
	{
		BaseComponent* component = VCreateComponent(gameAsset);
		if (component)
		{
			// *ITISSCAN* 23.11.2015. 
			// Not to good cast from GameAssetType structure to unsigned int...
			// Maybe in future better to make StringHash instead?
			pGameNode->AddComponent(component, (unsigned int)component->VGetGameAssetType(), component->GetCreateMode());
		}
		else
		{
			// If an error occurs, we kill the game node and bail. We could keep going, but the game node is will only be 
			// partially complete so it's not worth it. Note that the pGameNode instance will be destroyed because it
			// will fall out of scope with nothing else pointing to it.
			return StrongNodePtr();
		}
	}
	
	// *ITISSCAN* 23.11.2015. 
	// Not the best solution.
	// Send POST INIT event to game assets, in order to finish initialization properly.
	// May be some components depend on others.
	// In future we should to override Urho3D::Node class and implement our GameNode class.

	SendEvent("Game_Asset_Factory_Post_Init");

	return pGameNode;
}

void GameAssetFactory::ModifyNode(StrongNodePtr node, GameAsset* gameAsset)
{


}

StrongComponentPtr GameAssetFactory::VCreateComponent(GameAsset* gameAsset)
{
	GameAssetType GA_Type = gameAsset->GetType();
	StrongComponentPtr pComponent(m_ComponentFactory.Create((unsigned int)GA_Type));

	// initialize the component if we found one
	if (pComponent)
	{
		if (!pComponent->VInit(gameAsset))
		{
			URHO3D_LOGDEBUG("Component failed to initialize: " + String((unsigned int)GA_Type));
			return StrongComponentPtr();
		}
	}
	else
	{
		URHO3D_LOGDEBUG("Couldn't find GameAssetComponent id " + String((unsigned int)GA_Type));
		return StrongComponentPtr();
	}

	// pComponent will be NULL if the component wasn't found. 
	// This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	// For now it is ERROR.

	return pComponent;
}

	