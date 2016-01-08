// include engine headers
#include "AlphaEngineStd.h"

#include "GameAssetEngineSkybox.h"

// constant
const GameAssetType GameAssetEngineSkybox::g_Type = GAType_EngineSkybox;

GameAssetEngineSkybox::GameAssetEngineSkybox(Context* context) : BaseComponent(context)
{
    m_pNodeSkybox = NULL;              // Null static model
}

GameAssetEngineSkybox::GameAssetEngineSkybox() : BaseComponent()
{
   m_pNodeSkybox = NULL;              // Null static model
}

GameAssetEngineSkybox::~GameAssetEngineSkybox()
{

}

bool GameAssetEngineSkybox::VInit(const GameAsset* pGameAsset)
{
     // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();
    FileSystem* filesystem = g_pApp->GetFileSystem();

    // Set type and state to nothing for now
    m_GameAssetType = GAType_EngineSkybox;
    m_GameAssetState = GAState_None;

    // Get SkyboxModel
    SkyboxModel=pGameAsset->GetPhysicalModel();

    return true;
}

// Initialize
void GameAssetEngineSkybox::Initialize(void)
{
    // Get Attached node - preventing segfault problems
    Node * pThisNode = this->GetNode();

    if(!pThisNode)
    {
        return;
    }

    // Get resource cache
    ResourceCache* resCache = g_pApp->GetConstantResCache();

    // Create a model and string
    String SkyboxModelFile = String("Models/") + SkyboxModel+String(".mdl");

    // create a static model
    m_pNodeSkybox  = pThisNode->CreateComponent<Skybox>(Urho3D::CreateMode::LOCAL, pThisNode->GetID());

    // Set model and force default material loading
    m_pNodeSkybox->SetModel(resCache->GetResource<Model>(SkyboxModelFile));
    m_pNodeSkybox->ApplyMaterialList();
	
    // Set Default scale
    pThisNode->SetScale(2000.0f);

    return;
}
