#pragma once

#include "../PickupInterface.h"

class HealthPickup : public PickupInterface
{
public:
	static const GameAssetType g_Type;
	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

	virtual bool VInit(GameAsset* pData);
	virtual void VApply(WeakNodePtr pGameNode);
};
