#ifndef GameAssetRefrigerationUnit_H
#define GameAssetRefrigerationUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetRefrigerationUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetRefrigerationUnit, BaseComponent);
public:
    // Construct.
	GameAssetRefrigerationUnit(Context* context);
    GameAssetRefrigerationUnit();
    virtual ~GameAssetRefrigerationUnit(void);

	static const GameAssetType g_Type;

    // Base component overrided virtual functions
	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

private:

};

#endif // INTERACTIVE_H
