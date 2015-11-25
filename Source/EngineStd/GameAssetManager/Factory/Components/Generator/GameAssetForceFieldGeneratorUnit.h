#ifndef GameAssetForceFieldGeneratorUnit_H
#define GameAssetForceFieldGeneratorUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetForceFieldGeneratorUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetForceFieldGeneratorUnit, BaseComponent);

public:
    // Construct.
	GameAssetForceFieldGeneratorUnit(Context* context);
    GameAssetForceFieldGeneratorUnit();
    virtual ~GameAssetForceFieldGeneratorUnit(void);

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

private:

};

#endif // INTERACTIVE_H
