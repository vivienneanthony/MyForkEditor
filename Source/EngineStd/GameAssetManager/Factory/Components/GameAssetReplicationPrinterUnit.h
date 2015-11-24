#ifndef GameAssetReplicationPrinterUnit_H
#define GameAssetReplicationPrinterUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "../BaseComponent.h"

#include "GameAssetReplicationPrinterUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetReplicationPrinterUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetReplicationPrinterUnit, BaseComponent);

    public:
    // Construct.
    GameAssetReplicationPrinterUnit(Context* context);
    virtual ~GameAssetReplicationPrinterUnit(void);

    // Override - Virtuals
    bool VInit(GameAsset* pGameAsset){};

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
