// include engine headers
#include "EngineStd.h"

// header for specific component
#include "GameAssetEngineLight.h"

const GameAssetType GameAssetEngineLight::g_Type = GAType_EngineLight;

GameAssetEngineLight::GameAssetEngineLight(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetEngineLight::GameAssetEngineLight() : BaseComponent()
{
    // Set type and state to nothing for now
    m_GameAssetType=GAType_EngineLight;
    m_GameAssetState=GAState_None;
}

// Destructor
GameAssetEngineLight::~GameAssetEngineLight(void)
{
    return;
}

bool GameAssetEngineLight::VInit(GameAsset* pGameAsset)
{
    // Set type and state to nothing for now
    m_GameAssetType = GAType_EngineLight;
    m_GameAssetState = GAState_None;

    return true;
}

// Add components here needed
void GameAssetEngineLight::Initialize(void)
{
    // get attached node
    Node * thisNode = GetNode();

    // if node exist
    if(thisNode)
    {
        // Add light component
        Light * pnodeLight = thisNode->CreateComponent<Light>();

        // set brightness
        pnodeLight->SetBrightness(1.0f);
        pnodeLight->SetLightType(LIGHT_POINT);
    }

    return;
}
