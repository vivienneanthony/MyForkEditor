#include "EngineStd.h"
#include "AmmoComponent.h"

const GameAssetType AmmoPickup::g_Type = GAType_AmmoPickup;

AmmoPickup::AmmoPickup(Context* context) : PickupInterface(context)
{

}

AmmoPickup::AmmoPickup() : PickupInterface()
{

}


bool AmmoPickup::VInit(const GameAsset* pGameAsset)
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
