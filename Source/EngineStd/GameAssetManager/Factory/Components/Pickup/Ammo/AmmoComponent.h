#pragma once

#include "../PickupInterface.h"

class AmmoPickup : public PickupInterface
{
public:
	static const GameAssetType g_Type;
	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VApply(WeakNodePtr pGameNode);
};
