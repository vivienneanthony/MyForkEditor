#include "HangarsClientStd.h"

#include "EngineStd/Mainloop/Activity/ActivityManager.h"

#include "MainMenuView.h"
#include "UserInterface/MainMenuUI.h"
#include "Activities/Intro/IntroActivity.h"

#include "EngineStd/GameAssetManager/Factory/GameAssetFactory.h"
#include "EngineStd/GameAssetManager/GameAssetManager.h"

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

    // Load a game asset
    GameAsset * SphereTest = m_pGameAssetManager->FindGameAssetBySymbol("SphereTest");

    // Load a sphere
    if(SphereTest)
    {
        URHO3D_LOGINFO("Sphere created manually.");

        // create a sphere node
        StrongNodePtr SphereNode = m_pGameAssetFactory->CreateNode(SphereTest, 2);

        if(SphereNode)
        {
            m_pScene->AddChild(SphereNode);
        }
    }

    // Load a light
    GameAsset * EngineLight = m_pGameAssetManager->FindGameAssetBySymbol("EngineLight");

    if(EngineLight)
    {
        URHO3D_LOGINFO("Engine Light created manually.");

        // create a sphere node
        StrongNodePtr EngineLightNode = m_pGameAssetFactory->CreateNode(EngineLight, 3);

        if(EngineLightNode)
        {
            m_pScene->AddChild(EngineLightNode);
        }
    }

    return;
}
