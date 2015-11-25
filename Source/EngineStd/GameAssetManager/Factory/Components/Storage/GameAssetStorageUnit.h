#ifndef GameAssetStorageUnit_H
#define GameAssetStorageUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetStorageUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetStorageUnit, BaseComponent);

public:
    // Construct.
	GameAssetStorageUnit(Context* context);
    GameAssetStorageUnit();
    virtual ~GameAssetStorageUnit();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

private:

};

#endif // INTERACTIVE_H
