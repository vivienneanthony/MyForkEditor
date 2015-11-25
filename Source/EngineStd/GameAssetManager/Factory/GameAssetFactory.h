#pragma once

#include "../GameAsset.h"
#include "../GameAssetManager.h"                                         //needed to find game assets by symbol

#include "BaseComponent.h"

class GameAssetFactory : public Object
{
	URHO3D_OBJECT(GameAssetFactory, Object);

public:
	GameAssetFactory(Context* context_);
	~GameAssetFactory();

	// Node can be created on client side and server side.
	// On client side we get event from server, where is contained serversId.
	StrongNodePtr CreateNode(GameAsset* gameAsset, GameNodeId serversId);

	void ModifyNode(StrongNodePtr node, GameAsset* gameAsset);

	virtual StrongComponentPtr VCreateComponent(GameAsset* gameAsset);

	StrongNodePtr CreateNodeRecursive(GameAsset* gameAsset, GameNodeId serversId, Node * node, bool recursive);

    // Set the game asset manager
	void SetGameAssetManager (GameAssetManager * setGameAssetManager)
	{
	    m_pGameAssetManager = setGameAssetManager;

	    return;
	}

protected:
	GenericObjectFactory<BaseComponent, ComponentId> m_ComponentFactory;

private:
	GameNodeId GetNextGameNodeId() { ++m_LastGameNodeId; return m_LastGameNodeId; }

private:
	GameNodeId m_LastGameNodeId;

	String * GameAssetTypeToString(GameAssetType inputType);

	GameAssetManager * m_pGameAssetManager;
};
