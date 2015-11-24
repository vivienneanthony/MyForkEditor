// include engine headers
#include "EngineStd.h"

// interactive specific header
#include "GameAssetObject.h"

// header for specific component
#include "GameAssetRefrigerationUnit.h"

class GameAssetObject;

// Game Asset Component - Type
GameAssetRefrigerationUnit::GameAssetRefrigerationUnit(Context * context)
    :GameAssetObject(context)
     // m_Lifetime(0.0f)                            // inherited from base class
     // m_GameAssetType(GAType_None)                // inherited from base class
     // m_GameAssetState(GAState_None)              // inherited from base class
{
     // Set lifetime to 0
    m_Lifetime=0.0f;

    // Set type and state to nothing for now
    m_GameAssetType=GAType_RefrigerationUnit;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Register Factory
void GameAssetRefrigerationUnit::RegisterObject(Context* context)
{
    // Register
    context->RegisterFactory<GameAssetRefrigerationUnit>();

    return;
}

// Destructor
GameAssetRefrigerationUnit::~GameAssetRefrigerationUnit(void)
{
    return;
}
