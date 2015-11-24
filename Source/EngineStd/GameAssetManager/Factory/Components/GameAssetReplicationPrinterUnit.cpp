// include engine headers
#include "EngineStd.h"

// interactive specific header
#include "GameAssetObject.h"

// header for specific component
#include "GameAssetReplicationPrinterUnit.h"

class GameAssetObject;

// Game Asset Component - Type
GameAssetReplicationPrinterUnit::GameAssetReplicationPrinterUnit(Context * context)
    :BaseComponent()
{
     // Set lifetime to 0
    m_Lifetime=0.0f;

    // Set type and state to nothing for now
    m_GameAssetType=GAType_ReplicationPrinterUnit;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Register Factory
void GameAssetReplicationPrinterUnit::RegisterObject(Context* context)
{
    // Register
    context->RegisterFactory<GameAssetReplicationPrinterUnit>();

    return;
}

// Destructor
GameAssetReplicationPrinterUnit::~GameAssetReplicationPrinterUnit(void)
{
    return;
}
