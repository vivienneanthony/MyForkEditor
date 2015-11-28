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

    LoadDemoScene("Demo1");

    // Create a scene node for the camera, which we will move around
    // The camera will use default settings (1000 far clip distance, 45 degrees FOV, set aspect ratio automatically)
    m_pCameraNode = m_pScene->CreateChild("Camera");
    m_pCameraNode->CreateComponent<Camera>();

    m_pCameraNode->SetPosition(Vector3(30.0f,10.0f,30.0f));

    // Following code hidden line 116 cause crash
    SharedPtr<Viewport> viewport(new Viewport(context_, m_pScene, m_pCameraNode->GetComponent<Camera>()));

    // Set Viewport
    m_pRenderer->SetViewport(1, viewport);

    // Test Look at
    m_pCameraNode->LookAt(Vector3(0.0f,0.0f,0.0f));

    return;
}


// load specific file
bool MainMenuView::LoadDemoScene(String DemoFile)
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
        if(pPackageFile->Exists(String("GameDemos/")+DemoFile+String(".xml")))
        {
            // Set found Demo
            bDemoFileFound=true;
        }
    }

    // If package is not found or Demo is not found
    if(bPackageFileFound==false||bDemoFileFound==false)
    {

        URHO3D_LOGERROR ("Game Demo Loader - Error loading demo file..");

        return false;
    }

    // Package Entry
    const PackageEntry* pGameAssetsDemoEntry = pPackageFile->GetEntry(String("GameDemos/")+DemoFile+String(".xml"));

    // Set package
    File* pPackageData = new File(context_, pPackageFile, String("GameDemos/")+DemoFile+String(".xml"));

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
        URHO3D_LOGERROR ("Game Demo Loader - Problem loading oading demo scene file");

        return false;
    }

    // Get XML root if it exist, if not exit
    pugi::xml_node GameAssetRoot = doc.first_child();

    // If no root is found
    if(!GameAssetRoot)
    {
        URHO3D_LOGERROR ("Game Demo Loader - Problem loading oading demo scene file");

        return false;
    }

    unsigned int i=0;

    // For ... loop through each child
    for(pugi::xml_node NewGameAsset =  GameAssetRoot.first_child(); NewGameAsset; NewGameAsset =  NewGameAsset.next_sibling())
    {

        // Get attributes from xml
        const char* symbol = NewGameAsset.attribute("Symbol").as_string();
        float XPos = NewGameAsset.attribute("XPos").as_float();
        float YPos = NewGameAsset.attribute("YPos").as_float();
        float ZPos = NewGameAsset.attribute("ZPos").as_float();
        Quaternion rotation = Quaternion( NewGameAsset.attribute("Rotation").as_float());

        // Load a game asset
        GameAsset* LoadedGameAsset =  pAssetManager->FindGameAssetBySymbol(symbol);

        // Load a sphere
        if(LoadedGameAsset)
        {
            // create a sphere node
            StrongNodePtr LoadedGameAssetNode = pAssetFactory->CreateNode(LoadedGameAsset, i);

            if(LoadedGameAssetNode)
            {
                m_pScene->AddChild(LoadedGameAssetNode);

                LoadedGameAssetNode->SetPosition(Vector3(XPos,YPos,ZPos));

                i++;
            }


        }
    }

    // Safe delete
    SAFE_DELETE(pPackageData);
    SAFE_DELETE(pPackageFile);

    // Add addtional
    return true;
}
