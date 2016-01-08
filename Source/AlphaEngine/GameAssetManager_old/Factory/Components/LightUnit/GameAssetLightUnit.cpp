// include engine headers
#include "AlphaEngineStd.h"

// header for specific component
#include "GameAssetLightUnit.h"

const GameAssetType GameAssetLightUnit::g_Type = GAType_LightUnit;

GameAssetLightUnit::GameAssetLightUnit(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetLightUnit::GameAssetLightUnit() : BaseComponent()
{
    // Set type and state to nothing for now
    m_GameAssetType=GAType_LightUnit;
    m_GameAssetState=GAState_None;
}

// Destructor
GameAssetLightUnit::~GameAssetLightUnit(void)
{
    return;
}

bool GameAssetLightUnit::VInit(const GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_LightUnit;
	m_GameAssetState = GAState_None;


	return true;
}