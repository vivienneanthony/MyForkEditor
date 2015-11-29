#ifndef GameAssetReplicationPrinterUnit_H
#define GameAssetReplicationPrinterUnit_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetReplicationPrinterUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetReplicationPrinterUnit, BaseComponent);

public:
    // Construct.
	GameAssetReplicationPrinterUnit(Context* context);
    GameAssetReplicationPrinterUnit();
    virtual ~GameAssetReplicationPrinterUnit();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

private:

};

#endif // INTERACTIVE_H
