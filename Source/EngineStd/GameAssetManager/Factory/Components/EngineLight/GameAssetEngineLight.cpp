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
    m_GameAssetType = GAType_EngineLight;
    m_GameAssetState = GAState_None;

    // Set light node to null
    m_pNodeLight = NULL;
}

// Destructor
GameAssetEngineLight::~GameAssetEngineLight(void)
{
    return;
}

bool GameAssetEngineLight::VInit(const GameAsset* pGameAsset)
{
    // Set type and state to nothing for now
    m_GameAssetType = GAType_EngineLight;
    m_GameAssetState = GAState_None;

    return true;
}

// Add components here needed
void GameAssetEngineLight::Initialize(void)
{
    // Get Attached node - preventing segfault problems
    Node * thisNode = this->GetNode();

    if(!thisNode)
    {
        return;
    }

    // Create a light component
     m_pNodeLight = thisNode->CreateComponent<Light>();

     // Set brightness
     m_pNodeLight->SetBrightness(0.0f);
     m_pNodeLight->SetSpecularIntensity(0.0f);
     m_pNodeLight->SetLightType(LIGHT_DIRECTIONAL);

    return;
}
