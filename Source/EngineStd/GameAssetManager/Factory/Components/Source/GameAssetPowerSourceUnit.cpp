// include engine headers
#include "EngineStd.h"

// header for specific component
#include "GameAssetPowerSourceUnit.h"

const GameAssetType GameAssetPowerSourceUnit::g_Type = GAType_PowerSourceUnit;

GameAssetPowerSourceUnit::GameAssetPowerSourceUnit(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetPowerSourceUnit::GameAssetPowerSourceUnit() : BaseComponent()
{

}



// Destructor
GameAssetPowerSourceUnit::~GameAssetPowerSourceUnit(void)
{
    return;
}

bool GameAssetPowerSourceUnit::VInit(const GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_PowerSourceUnit;
	m_GameAssetState = GAState_None;


	return true;
}