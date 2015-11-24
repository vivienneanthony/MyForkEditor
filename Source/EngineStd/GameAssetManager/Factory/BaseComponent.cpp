#include "EngineStd.h"

#include "../GameAsset.h"


#include "BaseComponent.h"

BaseComponent::BaseComponent() : LogicComponent(g_pApp->GetContext())
    ,m_Lifetime(0.0f)
    ,m_GameAssetType(GAType_None)
    ,m_GameAssetState(GAState_None)
{
    m_CreateMode = Urho3D::CreateMode::LOCAL;
    m_bIsPostInit = false;

    VCreateAllDelegates();
}

BaseComponent::~BaseComponent()
{
    VDestroyAllDelegates();
}

void BaseComponent::VCreateAllDelegates()
{
    SubscribeToEvent("Game_Asset_Factory_Post_Init", URHO3D_HANDLER(BaseComponent, ComponentPostInitDelegate));

}

void BaseComponent::VDestroyAllDelegates()
{

}

void BaseComponent::ComponentPostInitDelegate(StringHash eventType, VariantMap& eventData)
{
    VPostInit();
    UnsubscribeFromEvent("Game_Asset_Factory_Post_Init");
}


