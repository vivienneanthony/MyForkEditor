// include engine headers
#include "EngineStd.h"

#include "GameAssetEngineObject.h"

// constant
const GameAssetType GameAssetEngineObject::g_Type = GAType_EngineObject;

GameAssetEngineObject::GameAssetEngineObject(Context* context) : BaseComponent(context)
{

}

GameAssetEngineObject::GameAssetEngineObject() : BaseComponent()
{

}

GameAssetEngineObject::~GameAssetEngineObject()
{

}

bool GameAssetEngineObject::VInit(GameAsset* pGameAsset)
{
    // Set type and state to nothing for now
	m_GameAssetType = GAType_EngineObject;
    m_GameAssetState = GAState_None;

    return true;
}

