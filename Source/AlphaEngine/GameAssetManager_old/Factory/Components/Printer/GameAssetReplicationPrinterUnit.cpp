// include engine headers
#include "AlphaEngineStd.h"

// header for specific component
#include "GameAssetReplicationPrinterUnit.h"

const GameAssetType GameAssetReplicationPrinterUnit::g_Type = GAType_ReplicationPrinterUnit;

GameAssetReplicationPrinterUnit::GameAssetReplicationPrinterUnit(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetReplicationPrinterUnit::GameAssetReplicationPrinterUnit() : BaseComponent()
{
  
}


// Destructor
GameAssetReplicationPrinterUnit::~GameAssetReplicationPrinterUnit(void)
{
    return;
}

bool GameAssetReplicationPrinterUnit::VInit(const GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_ReplicationPrinterUnit;
	m_GameAssetState = GAState_None;


	return true;
}

