#pragma once

#include "../GameAsset.h"
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

protected:
	GenericObjectFactory<BaseComponent, ComponentId> m_ComponentFactory;

private:
	GameNodeId GetNextGameNodeId() { ++m_LastGameNodeId; return m_LastGameNodeId; }

private:
	GameNodeId m_LastGameNodeId;
};