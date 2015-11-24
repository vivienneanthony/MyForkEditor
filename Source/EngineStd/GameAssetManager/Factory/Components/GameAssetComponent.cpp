// include engine headers
#include "EngineStd.h"

// interactive specific header
#include "GameAssetObject.h"

// header for specific component
#include "GameAssetComponent.h"

class GameAssetObject;

// Game Asset Component - Type
GameAssetComponent::GameAssetComponent(Context * context)
    :BaseComponent()
{
     // Set lifetime to 0
    m_Lifetime=0.0f;

    // Set type and state to nothing for now
    m_GameAssetType=GAType_Component;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Register Factory
void GameAssetComponent::RegisterObject(Context* context)
{
    // Register
    context->RegisterFactory<GameAssetComponent>();

    return;
}

// Destructor
GameAssetComponent::~GameAssetComponent(void)
{
    return;
}
