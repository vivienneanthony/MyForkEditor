#ifndef GameAssetObject_H
#define GameAssetObject_H

// include engine headers
#include "EngineStd.h"

// include the game asset manager specifically rc type
#include "EngineStd/GameAssetManager/GameAsset.h"

// Base game object
#include "../BaseComponent.h"

#include "GameAssetObject.h"

// Game Asset
class GameAssetObject : public BaseComponent
{
    // urho identifier
    URHO3D_OBJECT(GameAssetObject, BaseComponent);

public:
    // Construct.
    GameAssetObject(Context * context);

    virtual ~GameAssetObject();

    // Override - Virtuals
    bool VInit(GameAsset* pGameAsset){};

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize(void);

    // Handle update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);

    // return lifetime
    inline float GetLifetime(void)
    {
        return m_Lifetime;
    };

    // set Game Asset State
    inline GameAssetType GetGameAssetType(void)
    {
        return m_GameAssetType;
    }

    inline void SetGameAssetType(GameAssetType setType)
    {
        m_GameAssetType=setType;

        return;
    }

    // set Game Asset State
    inline GameAssetState GetGameAssetState(void)
    {
        return m_GameAssetState;
    }

    inline void SetGameAssetState(GameAssetState setState)
    {
        m_GameAssetState=setState;

        return;
    }

protected:
    float m_Lifetime;                    // lifetime
    GameAssetType m_GameAssetType;       // game asset type
    GameAssetState m_GameAssetState;     // set state


};

#endif // INTERACTIVE_H
