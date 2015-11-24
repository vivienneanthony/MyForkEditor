#ifndef GameAssetComponent_H
#define GameAssetComponent_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "../BaseComponent.h"

#include "GameAssetComponent.h"

class GameAssetObject;

// Game Asset
class GameAssetComponent : public BaseComponent
{
    URHO3D_OBJECT(GameAssetComponent, BaseComponent);

    public:
    // Construct.
    GameAssetComponent(Context* context);
    virtual ~GameAssetComponent(void);

// Override - Virtuals
    bool VInit(GameAsset* pGameAsset){};

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
