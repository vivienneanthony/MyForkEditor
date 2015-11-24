// include engine headers
#include "EngineStd.h"

// interactive specific header
#include "GameAssetObject.h"

// header for specific component
#include "GameAssetPowerSourceUnit.h"

class GameAssetObject;

// Game Asset Component - Type
GameAssetPowerSourceUnit::GameAssetPowerSourceUnit(Context * context)
    :GameAssetObject(context)
     // m_Lifetime(0.0f)                            // inherited from base class
     // m_GameAssetType(GAType_None)                // inherited from base class
     // m_GameAssetState(GAState_None)              // inherited from base class
{
     // Set lifetime to 0
    m_Lifetime=0.0f;

    // Set type and state to nothing for now
    m_GameAssetType=GAType_PowerSourceUnit;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Register Factory
void GameAssetPowerSourceUnit::RegisterObject(Context* context)
{
    // Register
    context->RegisterFactory<GameAssetPowerSourceUnit>();

    return;
}

// Destructor
GameAssetPowerSourceUnit::~GameAssetPowerSourceUnit(void)
{
    return;
}
