// include engine headers
#include "EngineStd.h"

#include "GameAssetEngineObject.h"

// constant
const GameAssetType GameAssetEngineObject::g_Type = GAType_EngineObject;

GameAssetEngineObject::GameAssetEngineObject(Context* context) : BaseComponent(context)
{
    m_pNodeStaticModel = NULL;              // Null static model
}

GameAssetEngineObject::GameAssetEngineObject() : BaseComponent()
{
   m_pNodeStaticModel = NULL;              // Null static model
}

GameAssetEngineObject::~GameAssetEngineObject()
{

}

bool GameAssetEngineObject::VInit(const GameAsset* pGameAsset)
{
     // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();
    FileSystem* filesystem = g_pApp->GetFileSystem();

    // Set type and state to nothing for now
    m_GameAssetType = GAType_EngineObject;
    m_GameAssetState = GAState_None;

    // Set flag
    if(pGameAsset->IsPhysical())
    {
        bPhysicalObject=true;
        PhysicalModel=pGameAsset->GetPhysicalModel();
    }

    return true;
}

// Initialize
void GameAssetEngineObject::Initialize(void)
{
    // Get Attached node - preventing segfault problems
    Node * pThisNode = GetNode();

    if(!pThisNode)
    {
        return;
    }

    // Get resource cache
    ResourceCache* resCache = g_pApp->GetConstantResCache();

    // Is Physical
    if(bPhysicalObject)
    {
        // Create a model and string
        String ModelFile = String("Models/") + PhysicalModel+String(".mdl");

        // create a static model
        m_pNodeStaticModel =  pThisNode->CreateComponent<StaticModel>();

        // Set model and force default material loading
        m_pNodeStaticModel->SetModel(resCache->GetResource<Model>(ModelFile));
        m_pNodeStaticModel->ApplyMaterialList();

        // Set Default paramet
        m_pNodeStaticModel->SetCastShadows(true);

    }

    return;
}
