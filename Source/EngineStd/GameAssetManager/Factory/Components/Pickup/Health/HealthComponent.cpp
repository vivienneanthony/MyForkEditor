#include "EngineStd.h"
#include "HealthComponent.h"

const GameAssetType HealthPickup::g_Type = GAType_HealthPickup;

bool HealthPickup::VInit(GameAsset* pGameAsset)
{
	return true;
}


void HealthPickup::VApply(WeakNodePtr pGameNode)
{
	if (pGameNode)
	{
		URHO3D_LOGINFO("Node from GameAsset. Applying health pickup to node " + pGameNode->GetName() + " node id " + pGameNode->GetID());
	}
}
