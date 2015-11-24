#ifndef GameAssetRefrigerationUnit_H
#define GameAssetRefrigerationUnit_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "GameAssetObject.h"
#include "GameAssetRefrigerationUnit.h"

class GameAssetObject;

// Game Asset
class GameAssetRefrigerationUnit : public GameAssetObject
{
    URHO3D_OBJECT(GameAssetRefrigerationUnit, GameAssetObject);

    public:
    // Construct.
    GameAssetRefrigerationUnit(Context* context);
    virtual ~GameAssetRefrigerationUnit(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
