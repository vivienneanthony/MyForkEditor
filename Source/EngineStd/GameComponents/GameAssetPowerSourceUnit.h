#ifndef GameAssetPowerSourceUnit_H
#define GameAssetPowerSourceUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "GameAssetObject.h"
#include "GameAssetPowerSourceUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetPowerSourceUnit : public GameAssetObject
{
    URHO3D_OBJECT(GameAssetPowerSourceUnit, GameAssetObject);

    public:
    // Construct.
    GameAssetPowerSourceUnit(Context* context);
    virtual ~GameAssetPowerSourceUnit(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
