#pragma once

#include "GameAssetFactory.h"
#include "../GameAsset.h"

class BaseComponent : public LogicComponent
{
    URHO3D_OBJECT(BaseComponent, LogicComponent);

    friend class GameAssetFactory;
public:
    BaseComponent();
    ~BaseComponent();

    // These functions are meant to be overridden by the implementation classes of the components.
    virtual bool VInit(GameAsset* pGameAsset) = 0;
    virtual void VPostInit(void)
    {
        m_bIsPostInit = true;
    }
    virtual void VOnChanged(void) { }

    // This function should be overridden by the interface class.
    virtual GameAssetType VGetGameAssetType(void) const
    {
        return GAType_None;
    }

    // Getters/Setters
    Urho3D::CreateMode GetCreateMode()
    {
        return m_CreateMode;
    }

    // Delegates
    void ComponentPostInitDelegate(StringHash eventType, VariantMap& eventData);

    //
    // Added sense it something shared among all base components for now
    //

    // Game Asset Related
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
    virtual void VCreateAllDelegates();
    virtual void VDestroyAllDelegates();


protected:
    Urho3D::CreateMode m_CreateMode;

    bool m_bIsPostInit;

    // Game Asset related
    float m_Lifetime;
    GameAssetType m_GameAssetType;
    GameAssetState m_GameAssetState;
};
