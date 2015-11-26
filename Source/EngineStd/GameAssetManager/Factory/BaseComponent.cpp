#include "EngineStd.h"

#include "../GameAsset.h"


#include "BaseComponent.h"

BaseComponent::BaseComponent(Context* context) : LogicComponent(context)
	,m_Lifetime(0.0f)
	,m_GameAssetType(GAType_None)
	,m_GameAssetState(GAState_None)
	,m_CreateMode(Urho3D::CreateMode::LOCAL)
	,m_bIsPostInit(false)
{

}

BaseComponent::BaseComponent() : LogicComponent(g_pApp->GetContext())
    ,m_Lifetime(0.0f)
    ,m_GameAssetType(GAType_None)
    ,m_GameAssetState(GAState_None)
	,m_CreateMode(Urho3D::CreateMode::LOCAL)
	,m_bIsPostInit(false)
{
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
	UnsubscribeFromAllEvents();
}

void BaseComponent::FixedUpdate(float timeStep)
{
	m_Lifetime += timeStep;

	// Call subclassed update function
	VUpdateDelegate(timeStep);
}




void BaseComponent::Initialize()
{
	// Call subclassed initialize function
	VInitializeDelegate();
}



void BaseComponent::ComponentPostInitDelegate(StringHash eventType, VariantMap& eventData)
{
    VPostInit();
    UnsubscribeFromEvent("Game_Asset_Factory_Post_Init");
}




