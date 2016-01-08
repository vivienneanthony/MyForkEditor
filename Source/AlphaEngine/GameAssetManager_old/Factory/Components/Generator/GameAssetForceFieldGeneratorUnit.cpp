// include engine headers
#include "AlphaEngineStd.h"

// header for specific component
#include "GameAssetForceFieldGeneratorUnit.h"

const GameAssetType GameAssetForceFieldGeneratorUnit::g_Type = GAType_ForcefieldGenerator;

GameAssetForceFieldGeneratorUnit::GameAssetForceFieldGeneratorUnit(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetForceFieldGeneratorUnit::GameAssetForceFieldGeneratorUnit() : BaseComponent()
{

}


// Destructor
GameAssetForceFieldGeneratorUnit::~GameAssetForceFieldGeneratorUnit(void)
{
    return;
}

bool GameAssetForceFieldGeneratorUnit::VInit(const GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_ForcefieldGenerator;
	m_GameAssetState = GAState_None;


	return true;
}
