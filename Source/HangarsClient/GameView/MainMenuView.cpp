#include "HangarsClientStd.h"

#include "EngineStd/Mainloop/Activity/ActivityManager.h"
#include "EngineStd/GameLogic/BaseGameLogic.h"

#include "EngineStd/GameAssetManager/Factory/GameAssetFactory.h"
#include "EngineStd/GameAssetManager/GameAssetManager.h"

#include "MainMenuView.h"
#include "UserInterface/MainMenuUI.h"
#include "Activities/Intro/IntroActivity.h"


MainMenuView::MainMenuView(Context* context, Renderer* renderer, bool intro) : HumanView(context, renderer)
{
    m_pMainMenu = SharedPtr<IScreenElement>(new MainMenuUI(context));
    VPushElement(m_pMainMenu);
    m_pMainMenu->VSetVisible(false);

    VInitializeAllDelegates();

    if (intro)
    {
        StrongActivityPtr intro = SharedPtr<Activity>(new IntroActivity(context_));
        m_pActivityManager->AttachActivity(intro);
    }
    else
    {
        SendEvent("Finish_Introduction_Part");
    }

}

MainMenuView::~MainMenuView()
{


}

void  MainMenuView::VInitializeAllDelegates()
{
    HumanView::VInitializeAllDelegates();
    SubscribeToEvent("Finish_Introduction_Part", URHO3D_HANDLER(MainMenuView, FinishIntroductionPartDelegate));

}

void  MainMenuView::VDestroyAllDelegates()
{
    HumanView::VDestroyAllDelegates();
}

void  MainMenuView::FinishIntroductionPartDelegate(StringHash eventType, VariantMap& eventData)
{
    if (m_pMainMenu)
    {
        m_pMainMenu->VOnRestore();
    }

    m_pMainMenu->VSetVisible(true);

    UnsubscribeFromEvent("Finish_Introduction_Part");

    // Test loading a scene
    CreateManualScene();
}

void MainMenuView::CreateManualScene(void)
{
    // Update scene
    m_pScene = SharedPtr<Scene>(new Scene(context_));

    // Add octree
    m_pScene-> CreateComponent<Octree>();

    // Load a demo file
    LoadDemoScene("Demo1");

    // Create a scene node for the camera, which we will move around
    // The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
    m_pCameraNode = m_pScene->CreateChild(String("Camera").ToHash(), CreateMode::LOCAL);
    m_pCameraNode->CreateComponent<Camera>();

    // Set Position
    m_pCameraNode->SetPosition(Vector3(300.0f,40.0f,600.0f));

    // Following code hidden line 116 cause crash
    SharedPtr<Viewport> m_pViewport(new Viewport(context_, m_pScene, m_pCameraNode->GetComponent<Camera>()));

    // Set Viewport
    m_pRenderer->SetViewport(1, m_pViewport);

    // Test Look at
    m_pCameraNode->LookAt(Vector3(0.0f,-0.0f,0.0f));
    m_pCameraNode->GetComponent<Camera>()->SetFarClip(2000.0);

    return;
}


// load specific file
bool MainMenuView::LoadDemoScene(String demoFile)
{
    // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();
    FileSystem* filesystem = g_pApp->GetFileSystem();
    GameAssetManager* pAssetManager = g_pApp->GetGameLogic()->GetGameAssetManager();
    GameAssetFactory* pAssetFactory = g_pApp->GetGameLogic()->GetGameAssetFactory();

    // Create a empty
    Vector<String> datafiles;

    // Use package file
    PackageFile * pPackageFile = new PackageFile(context_);

    // Package found bool;
    bool bPackageFileFound = pPackageFile->Open("GameData.pak");

    // Found Demo found bool
    bool bDemoFileFound = false;

    // If packaged file is found continue
    if(bPackageFileFound == true)
    {
        // Use this package if found and matches
        if(pPackageFile->Exists(String("GameDemos/") + demoFile + String(".xml")))
        {
            // Set found Demo
            bDemoFileFound = true;
        }
    }

    // If package is not found or Demo is not found
    if(bPackageFileFound == false || bDemoFileFound==false)
    {

        URHO3D_LOGERROR ("Game Demo Loader - Error loading demo file..");

        return false;
    }

    // Package Entry
    const PackageEntry* pGameAssetsDemoEntry = pPackageFile->GetEntry(String("GameDemos/") + demoFile + String(".xml"));

    // Set package
    File* pPackageData = new File(context_, pPackageFile, String("GameDemos/") + demoFile + String(".xml"));

    // Read content to memory
    unsigned char* RetrievedDemoFile = NULL;

    RetrievedDemoFile = new unsigned char[pGameAssetsDemoEntry->size_];

    // Read data from file
    pPackageData->Read(&RetrievedDemoFile[0], pGameAssetsDemoEntry->size_);

    // Create new xml document - Pugixml friendly
    pugi::xml_document doc;

    char* buffer = static_cast<char*>(pugi::get_memory_allocation_function()(pGameAssetsDemoEntry->size_));
    memcpy(buffer, RetrievedDemoFile, pGameAssetsDemoEntry->size_);

    pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, pGameAssetsDemoEntry->size_);

    // Safe delete memory not in use
    delete [] RetrievedDemoFile;

    // Exit if error
    if (result.status != pugi::status_ok)
    {
        URHO3D_LOGERROR ("Game Demo Loader - Problem loading demo scene file");
        return false;
    }

    // Get XML root if it exist, if not exit
    pugi::xml_node GameAssetRoot = doc.first_child();

    // If no root is found
    if(!GameAssetRoot)
    {
        URHO3D_LOGERROR ("Game Demo Loader - Problem loading demo scene file");

        return false;
    }

    unsigned int i = 0;

    // For ... loop through each child
    for(pugi::xml_node NewGameAsset =  GameAssetRoot.first_child(); NewGameAsset; NewGameAsset = NewGameAsset.next_sibling())
    {
        // Get attributes from xml
        const char* pSymbol = NewGameAsset.attribute("Symbol").as_string();
        const char* pNodeName = NewGameAsset.attribute("NodeName").as_string();
        float XPos = NewGameAsset.attribute("XPos").as_float();
        float YPos = NewGameAsset.attribute("YPos").as_float();
        float ZPos = NewGameAsset.attribute("ZPos").as_float();
        float XRot = NewGameAsset.attribute("XRot").as_float();
        float YRot = NewGameAsset.attribute("YRot").as_float();
        float ZRot = NewGameAsset.attribute("ZRot").as_float();
        float WRot = NewGameAsset.attribute("WRot").as_float();

        // get rotation
        Quaternion Rot = Quaternion(WRot, XRot, YRot, ZRot);

        // Load a game asset
        GameAsset* pLoadedGameAsset =  pAssetManager->FindGameAssetBySymbol(pSymbol);

        // Load game asset pointer
        if(pLoadedGameAsset)
        {
            // Create a node
            StrongNodePtr pLoadedGameAssetNode = pAssetFactory->CreateNode(pLoadedGameAsset, INVALID_GAME_NODE_ID);

            if(pLoadedGameAssetNode)
            {
                m_pScene->AddChild(pLoadedGameAssetNode);

                // Set name if its in the xml
                if(pNodeName)
                {
                    pLoadedGameAssetNode->SetName(String(pNodeName));
                }

                // Change node rotation and position
                pLoadedGameAssetNode->SetPosition(Vector3(XPos,YPos,ZPos));
                pLoadedGameAssetNode->SetRotation(Rot);

                // ModifyNode based on input information
                pAssetFactory->ModifyNode(pLoadedGameAssetNode, pLoadedGameAsset, NewGameAsset );
            }
        }

        // Load game asset pointer
        if(String(pSymbol)==String("Grouped"))
        {
            // Create a node
            StrongNodePtr pGroupGameAssets = pAssetFactory->CreateNodeEmpty(INVALID_GAME_NODE_ID);

            // if node as created
            if(pGroupGameAssets)
            {
                // Loop through each child
                for (pugi::xml_node NewGroupedGameAsset : NewGameAsset.children())
                {
                    // Get attributes from xml
                    const char* pChild_Symbol = NewGroupedGameAsset.attribute("Symbol").as_string();
                    const char* pChild_NodeName = NewGameAsset.attribute("NodeName").as_string();
                    float Child_XPos = NewGroupedGameAsset.attribute("XPos").as_float();
                    float Child_YPos = NewGroupedGameAsset.attribute("YPos").as_float();
                    float Child_ZPos = NewGroupedGameAsset.attribute("ZPos").as_float();
                    float Child_XRot = NewGroupedGameAsset.attribute("XRot").as_float();
                    float Child_YRot = NewGroupedGameAsset.attribute("YRot").as_float();
                    float Child_ZRot = NewGroupedGameAsset.attribute("ZRot").as_float();
                    float Child_WRot = NewGroupedGameAsset.attribute("WRot").as_float();

                    // get rotation
                    Quaternion Child_Rot = Quaternion(Child_WRot, Child_XRot,Child_YRot,Child_ZRot);

                    // Load a game asset
                    GameAsset* pChild_LoadedGameAsset =  pAssetManager->FindGameAssetBySymbol(pChild_Symbol);

                    // Load a sphere
                    if(pChild_LoadedGameAsset)
                    {
                        // create a sphere node
                        StrongNodePtr pChild_LoadedGameAssetNode = pAssetFactory->CreateNode(pChild_LoadedGameAsset, INVALID_GAME_NODE_ID);

                        if(pChild_LoadedGameAssetNode)
                        {
                            pGroupGameAssets->AddChild(pChild_LoadedGameAssetNode);

                            // Set name if it's in the xml
                            if(pChild_NodeName)
                            {
                                pChild_LoadedGameAssetNode->SetName(String(pChild_NodeName));
                            }

                            // Change node rotation and position
                            pChild_LoadedGameAssetNode->SetPosition(Vector3(Child_XPos,Child_YPos,Child_ZPos));
                            pChild_LoadedGameAssetNode->SetRotation(Child_Rot);

                             // ModifyNode based on input information
                            pAssetFactory->ModifyNode(pChild_LoadedGameAssetNode, pChild_LoadedGameAsset, NewGroupedGameAsset );
                        }
                    }
                }
            }

            // if node is empty then delete
            if(pGroupGameAssets->GetNumChildren()==0)
            {
                pGroupGameAssets->Remove();
            }
            else
            {
                // group to scene
                m_pScene->AddChild(pGroupGameAssets);

                // Change node rotation and position
                pGroupGameAssets->SetPosition(Vector3(XPos,YPos,ZPos));
                pGroupGameAssets->SetRotation(Rot);

            }
        }
    }

    // Safe delete
    SAFE_DELETE(pPackageData);
    SAFE_DELETE(pPackageFile);

    // Add addtional
    return true;
}
