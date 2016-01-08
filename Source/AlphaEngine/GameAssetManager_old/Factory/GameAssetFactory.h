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
	StrongNodePtr CreateNode(const GameAsset* gameAsset, const GameNodeId serversId);

    // Modify node
	void ModifyNode(StrongNodePtr node, const GameAsset* gameAsset);
	void ModifyNode(StrongNodePtr pChildNode, const GameAsset* gameAsset, const pugi::xml_node GameAssetChild);

	virtual StrongComponentPtr VCreateComponent(const GameAsset* gameAsset);

    // Additional node creation
	StrongNodePtr CreateNodeRecursive(const GameAsset* gameAsset, const GameNodeId serversId, Node* node, bool recursive);
	StrongNodePtr CreateNodeEmpty(const GameNodeId serversId);

    // Set the game asset manager
	void SetGameAssetManager (GameAssetManager* setGameAssetManager) { m_pGameAssetManager = setGameAssetManager; }

protected:
	GameNodeId GetNextGameNodeId() { ++m_LastGameNodeId; return m_LastGameNodeId; }
	String* GameAssetTypeToString(GameAssetType inputType);


protected:
	GenericObjectFactory<BaseComponent, ComponentId> m_ComponentFactory;
	GameNodeId m_LastGameNodeId;
	GameAssetManager * m_pGameAssetManager;
};
