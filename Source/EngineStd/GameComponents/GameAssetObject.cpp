// include engine headers
#include "EngineStd.h"

// interactive specific header
#include "GameAssetObject.h"

GameAssetObject::GameAssetObject(Context * context)
    :LogicComponent(context)
     ,m_Lifetime(0.0f)
     ,m_GameAssetType(GAType_None)
     ,m_GameAssetState(GAState_None)
{
     // Set lifetime to 0
    m_Lifetime=0.0f;

    // Set type and state to nothing for now
    m_GameAssetType=GAType_None;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Initialization
void GameAssetObject::Initialize(void)
{
    return;
}

// Register
void GameAssetObject::RegisterObject(Context* context)
{
    // Register
    context->RegisterFactory<GameAssetObject>();

    return;

}

// Fixed update
void GameAssetObject::FixedUpdate(float timeStep)
{
    m_Lifetime+=timeStep;

    return;
}


// Destructor
GameAssetObject::~GameAssetObject(void)
{
    return;
}


