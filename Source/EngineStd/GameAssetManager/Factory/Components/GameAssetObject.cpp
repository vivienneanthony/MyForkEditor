// include engine headers
#include "EngineStd.h"

#include "GameAssetManager/GameAsset.h"

#include "GameAssetObject.h"

// constant
const GameAssetType GameAssetObject::g_Type = GAType_Object;

GameAssetObject::GameAssetObject(Context* context) : BaseComponent(context)
{

}

GameAssetObject::GameAssetObject() : BaseComponent()
{

}

GameAssetObject::~GameAssetObject()
{

}

bool GameAssetObject::VInit(GameAsset* pGameAsset)
{
    // Set type and state to nothing for now
	m_GameAssetType = GAType_Object;
    m_GameAssetState = GAState_None;

    return true;
}

