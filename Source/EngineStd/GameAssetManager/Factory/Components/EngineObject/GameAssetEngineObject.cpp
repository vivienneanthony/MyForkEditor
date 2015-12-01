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
    Node * thisNode = GetNode();

    if(!thisNode)
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
        StaticModel* m_pGameNodeModel = this->GetNode()->CreateComponent<StaticModel>();

        // Set model and force default material loading
        m_pGameNodeModel->SetModel(resCache->GetResource<Model>(ModelFile));
        m_pGameNodeModel->ApplyMaterialList();

        // Set Default paramet
        m_pGameNodeModel->SetCastShadows(true);
    }

    return;
}
