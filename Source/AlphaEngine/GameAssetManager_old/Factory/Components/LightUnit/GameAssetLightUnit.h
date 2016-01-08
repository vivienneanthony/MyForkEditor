#ifndef GameAssetLightUnit_H
#define GameAssetLightUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetLightUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetLightUnit, BaseComponent);
public:
    // Construct.
	GameAssetLightUnit(Context* context);
    GameAssetLightUnit();
    virtual ~GameAssetLightUnit();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

private:

};

#endif // INTERACTIVE_H
