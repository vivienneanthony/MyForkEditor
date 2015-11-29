#pragma once

#include "GameAssetFactory.h"

#include "../GameAsset.h"

class BaseComponent : public LogicComponent
{
	URHO3D_OBJECT(BaseComponent, LogicComponent);

	friend class GameAssetFactory;
public:
	BaseComponent(Context* context);
	BaseComponent();
	~BaseComponent();

	// These functions are meant to be overridden by the implementation classes of the components.
	// Handled by GameAsset factory
	virtual bool VInit(const GameAsset* pGameAsset) = 0;
	virtual void VPostInit(void) { m_bIsPostInit = true; }
	virtual void VOnChanged(void) { }

	// Handle startup. Called by LogicComponent base class.
	virtual void Initialize(void);
	// Handle update. Called by LogicComponent base class.
	virtual void FixedUpdate(float timeStep);

	// There are could be overrided by subclasses.
	// Handle Urho3D::Logic component update and initialize calls.
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	// Delegates
	void ComponentPostInitDelegate(StringHash eventType, VariantMap& eventData);

	// Getters/Setters
	// Added sense it something shared among all base components for now
	// Game Asset Related
	inline float GetLifetime(void) { return m_Lifetime; };
	GameAssetType GetGameAssetType(void) const { return m_GameAssetType; }
	GameAssetState GetGameAssetState(void) const { return m_GameAssetState; }
	inline Urho3D::CreateMode GetCreateMode() { return m_CreateMode; }
	inline void SetGameAssetType(GameAssetType setType) { m_GameAssetType = setType; }
    inline void SetGameAssetState(GameAssetState setState) { m_GameAssetState = setState; }

protected:
	// Delegates
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
