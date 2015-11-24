#ifndef GameAssetForceFieldGeneratorUnit_H
#define GameAssetForceFieldGeneratorUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "GameAssetObject.h"
#include "GameAssetForceFieldGeneratorUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetForceFieldGeneratorUnit : public GameAssetObject
{
    URHO3D_OBJECT(GameAssetForceFieldGeneratorUnit, GameAssetObject);

    public:
    // Construct.
    GameAssetForceFieldGeneratorUnit(Context* context);
    virtual ~GameAssetForceFieldGeneratorUnit(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
