// include engine headers
#include "EngineStd.h"

// header for specific component
#include "GameAssetEngineCamera.h"

const GameAssetType GameAssetEngineCamera::g_Type = GAType_EngineCamera;

GameAssetEngineCamera::GameAssetEngineCamera(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetEngineCamera::GameAssetEngineCamera() : BaseComponent()
{
    // Set type and state to nothing for now
    m_GameAssetType=GAType_EngineCamera;
    m_GameAssetState=GAState_None;
}

// Destructor
GameAssetEngineCamera::~GameAssetEngineCamera(void)
{
    return;
}

bool GameAssetEngineCamera::VInit(const GameAsset* pGameAsset)
{
    // Set type and state to nothing for now
    m_GameAssetType = GAType_EngineCamera;
    m_GameAssetState = GAState_None;

    return true;
}

// Add components here needed
void GameAssetEngineCamera::Initialize(void)
{
    // get attached node
    Node * thisNode = GetNode();

    // if node exist
    if(thisNode)
    {
        // Add Camera component
        Camera * pnodeCamera = thisNode->CreateComponent<Camera>();
    }

    return;
}
