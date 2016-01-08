#pragma once

#include "../PickupInterface.h"

class AmmoPickup : public PickupInterface
{
public:
	static const GameAssetType g_Type;
	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

	AmmoPickup(Context* context);
	AmmoPickup();

	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VApply(WeakNodePtr pGameNode);
};
