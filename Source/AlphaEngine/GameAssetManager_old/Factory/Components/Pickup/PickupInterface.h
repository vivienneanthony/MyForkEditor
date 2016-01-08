#pragma once

#include "../../BaseComponent.h"

class PickupInterface : public BaseComponent
{
public:
	PickupInterface(Context* context);
	PickupInterface();

	// Pickup interface. Apply picked item to Node.
	virtual void VApply(WeakNodePtr pGameNode) = 0;
};
