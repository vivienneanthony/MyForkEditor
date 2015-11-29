#ifndef GameAssetPowerSourceUnit_H
#define GameAssetPowerSourceUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetPowerSourceUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetPowerSourceUnit, BaseComponent);
public:
    // Construct.
	GameAssetPowerSourceUnit(Context* context);
    GameAssetPowerSourceUnit();
    virtual ~GameAssetPowerSourceUnit(void);

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

private:

};

#endif // INTERACTIVE_H
