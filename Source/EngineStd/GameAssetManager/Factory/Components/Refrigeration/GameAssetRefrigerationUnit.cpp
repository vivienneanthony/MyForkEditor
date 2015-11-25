// include engine headers
#include "EngineStd.h"

// header for specific component
#include "GameAssetRefrigerationUnit.h"

const GameAssetType GameAssetRefrigerationUnit::g_Type = GAType_RefrigerationUnit;

GameAssetRefrigerationUnit::GameAssetRefrigerationUnit(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetRefrigerationUnit::GameAssetRefrigerationUnit() : BaseComponent()
{
   
    // Set type and state to nothing for now
    m_GameAssetType=GAType_RefrigerationUnit;
    m_GameAssetState=GAState_None;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

// Destructor
GameAssetRefrigerationUnit::~GameAssetRefrigerationUnit(void)
{
    return;
}

bool GameAssetRefrigerationUnit::VInit(GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_RefrigerationUnit;
	m_GameAssetState = GAState_None;


	return true;
}

