#ifndef GameAssetComponent_H
#define GameAssetComponent_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "GameAssetObject.h"
#include "GameAssetComponent.h"

class GameAssetObject;

// Game Asset
class GameAssetComponent : public GameAssetObject
{
    URHO3D_OBJECT(GameAssetComponent, GameAssetObject);

    public:
    // Construct.
    GameAssetComponent(Context* context);
    virtual ~GameAssetComponent(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
