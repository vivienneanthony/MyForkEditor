// include engine headers
#include "EngineStd.h"

// header for specific component
#include "GameAssetStorageUnit.h"

// constant
const GameAssetType GameAssetStorageUnit::g_Type = GAType_StorageUnit;

// Game Asset Component - Type
GameAssetStorageUnit::GameAssetStorageUnit(Context* context) : BaseComponent(context)
{

}


// Game Asset Component - Type
GameAssetStorageUnit::GameAssetStorageUnit() : BaseComponent()
{

}


// Destructor
GameAssetStorageUnit::~GameAssetStorageUnit()
{

}

bool GameAssetStorageUnit::VInit(GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_StorageUnit;
	m_GameAssetState = GAState_None;


	return true;
}

