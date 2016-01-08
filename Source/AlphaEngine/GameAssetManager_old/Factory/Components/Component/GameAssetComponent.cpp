// include engine headers
#include "AlphaEngineStd.h"

// header for specific component
#include "GameAssetComponent.h"

const GameAssetType GameAssetComponent::g_Type = GAType_Component;

GameAssetComponent::GameAssetComponent(Context* context) : BaseComponent(context)
{

}

// Game Asset Component - Type
GameAssetComponent::GameAssetComponent() : BaseComponent()
{

}

// Destructor
GameAssetComponent::~GameAssetComponent()
{

}

bool GameAssetComponent::VInit(const GameAsset* pGameAsset)
{
	// Set type and state to nothing for now
	m_GameAssetType = GAType_Component;
	m_GameAssetState = GAState_None;


	return true;
}
