#include "EngineStd.h"
#include "AmmoComponent.h"

const GameAssetType AmmoPickup::g_Type = GAType_AmmoPickup;

bool AmmoPickup::VInit(GameAsset* pGameAsset)
{
	return true;
}


void AmmoPickup::VApply(WeakNodePtr pGameNode)
{
	if (pGameNode)
	{
		URHO3D_LOGINFO("Node from GameAsset. Applying ammo pickup to node " + pGameNode->GetName() + " node id " + pGameNode->GetID());
	}
}
