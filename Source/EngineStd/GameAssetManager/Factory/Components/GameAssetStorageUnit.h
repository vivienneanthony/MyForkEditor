#ifndef GameAssetStorageUnit_H
#define GameAssetStorageUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "../BaseComponent.h"

#include "GameAssetStorageUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetStorageUnit : public BaseComponent
{
    URHO3D_OBJECT(GameAssetStorageUnit, BaseComponent);

    public:
    // Construct.
    GameAssetStorageUnit(Context* context);
    virtual ~GameAssetStorageUnit(void);

    // Override - Virtuals
    bool VInit(GameAsset* pGameAsset){};

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
