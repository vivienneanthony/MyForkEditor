#ifndef GameAssetLightUnit_H
#define GameAssetLightUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "GameAssetObject.h"
#include "GameAssetLightUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetLightUnit : public GameAssetObject
{
    URHO3D_OBJECT(GameAssetLightUnit, GameAssetObject);

    public:
    // Construct.
    GameAssetLightUnit(Context* context);
    virtual ~GameAssetLightUnit(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
