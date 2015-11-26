
#include "EngineStd.h"

#include "UserInterface/HumanView/HumanView.h"

#include "Mainloop/Activity/ActivityManager.h"

#include "Resources/ResourceCache.h"

#include "GameAssetManager/Factory/GameAssetFactory.h"

#include "LevelManager/LevelManager.h"
#include "GameAssetManager/GameAssetManager.h"

#include "BaseGameLogic.h"

// ----------------------------------------------------------
// Constructor/destructor
// ----------------------------------------------------------

BaseGameLogic::BaseGameLogic(Context *context) : IGameLogic(context)
{
    m_pActivityManager = NULL;
    m_pLevelManager = NULL;
    m_pGameAssetFactory = NULL;

    m_bIsRenderDiagnostic = false;

    m_HumanPlayersAttached = 0;
    m_ExpectedPlayers = 0;
    m_HumanGamesLoaded = 0;



    m_State = BGS_Invalid;
}

BaseGameLogic::~BaseGameLogic()
{

}

// ----------------------------------------------------------
// Init, run, shutdown
// ----------------------------------------------------------

bool BaseGameLogic::VInitialize()
{
    m_State = BGS_Initializing;

    // Change random seed
    SetRandomSeed(GetRandomSeed());

    m_pActivityManager = new ActivityManager(context_);

    m_pLevelManager = new LevelManager(context_);

    // New Game Asset Manager
    m_pGameAssetFactory = new GameAssetFactory(context_);

    m_pGameAssetManager = new GameAssetManager(context_);

    m_pGameAssetManager->Init();

    URHO3D_LOGINFO ("Game Asset Manager Initialized");

    // Give factory access to to the game assets
    m_pGameAssetFactory->SetGameAssetManager(m_pGameAssetManager);

    URHO3D_LOGINFO("Game Asset Manager assigned to Factory");

    // load game assets temporary
    m_pGameAssetManager->LoadGameAssets();

    // Test info string
    String Message= String("Game Asset Manager Loaded ") +String(m_pGameAssetManager->GetTotalGameAssets())+ String(" Game Assets");

    URHO3D_LOGINFO (Message);

    InitializeComponents();

    InitializeAllDelegates();

    return true;
}

void BaseGameLogic::VOnUpdate(float timeStep)
{
    m_Lifetime += timeStep;
    switch (m_State)
    {
    case BGS_Initializing:
    {
        // If we get to here we're ready to attach players
        VChangeState(BGS_MainMenu);
        break;
    }

    case BGS_MainMenu:
    {

        break;
    }


    case BGS_WaitingForPlayers:
    {
        if (m_ExpectedPlayers == m_HumanPlayersAttached)
        {
            // The server sends us the level name as a part of the login message.
            // We have to wait until it arrives before loading the level
            if (!g_pApp->GetGameOptions().m_Level.Empty())
            {
                VChangeState(BGS_LoadingGameEnvironment);
            }
        }
        break;
    }

    case BGS_WaitingForPlayersToLoadEnvironment:
    {
        if (m_ExpectedPlayers <= m_HumanGamesLoaded)
        {
            VChangeState(BGS_SpawningPlayersActors);
        }

        break;
    }

    case BGS_SpawningPlayersActors:
    {
        VChangeState(BGS_Running);
        break;
    }

    case BGS_Running:
    {
        m_pActivityManager->UpdateActivities(timeStep);
        break;
    }
    }

    // update all game views
    for (GameViewList::Iterator it = m_GameViews.Begin(); it != m_GameViews.End(); ++it)
    {
        (*it)->VOnUpdate(timeStep);
    }
}

void BaseGameLogic::VShutdown()
{
    // Call virtual shut down procedure.
    for (GameViewList::Iterator it = m_GameViews.Begin(); it != m_GameViews.End(); it++)
    {
        (*it)->VShutdown();
    }

    // Delete each game view from list.
    while (!m_GameViews.Empty())
    {
        m_GameViews.PopFront();
    }


    // delete

    SAFE_DELETE(m_pGameAssetFactory);

    SAFE_DELETE(m_pGameAssetManager);


    m_pLevelManager->Shutdown();

    SAFE_DELETE(m_pLevelManager);
    SAFE_DELETE(m_pActivityManager);

    DestroyAllDelegates();

    URHO3D_LOGINFO(String("Game logic life time is - ")
                   + String(m_Lifetime / 3600.0f) + String(" hours ")
                   + String(m_Lifetime / 60.0f) + String(" minutes ")
                   + String(m_Lifetime) + String(" seconds"));

    m_State = BGS_Invalid;
}



// ----------------------------------------------------------
// Actor manipulations
// ----------------------------------------------------------

void BaseGameLogic::VDestroyGameNode(const GameNodeId gameAssetId)
{
    Node* node = m_pScene->GetNode(gameAssetId);
    if (node)
    {
        m_pScene->RemoveChild(node);
    }
}

// ----------------------------------------------------------
// Handle game state
// ----------------------------------------------------------

void BaseGameLogic::VChangeState(enum BaseGameState newState)
{

    if (newState == BGS_WaitingForPlayers)
    {
        // Get rid of the Main Menu...
        SharedPtr<IGameView> it;
        it = m_GameViews.Front();
        it->VShutdown();
        m_GameViews.PopFront();

        // Note: Split screen support would require this to change!
        m_ExpectedPlayers = 1;
    }
    else if (newState == BGS_LoadingGameEnvironment)
    {
        m_State = newState;
        if (!g_pApp->VLoadGame())
        {
            URHO3D_LOGERROR("The game failed to load.");
            g_pApp->AbortGame();
        }
        else
        {
            VChangeState(BGS_WaitingForPlayersToLoadEnvironment);			// we must wait for all human player to report their environments are loaded.
            return;
        }
    }




    m_State = newState;
}


bool BaseGameLogic::VLoadGame(String levelResource)
{
    SharedPtr<File> file = g_pApp->GetConstantResCache()->GetFile(levelResource);
    m_pScene = SharedPtr<Scene>(new Scene(context_));

    // initialize all human views
    for (auto it = m_GameViews.Begin(); it != m_GameViews.End(); ++it)
    {
        SharedPtr<IGameView> pView = *it;
        if (pView->VGetType() == GameView_Human)
        {
            SharedPtr<HumanView> pHumanView = StaticCast<HumanView, IGameView>(pView);
            pHumanView->LoadGame(file, m_pScene);
        }
    }

    // call the delegate load function
    if (!VLoadGameDelegate(levelResource))
        return false;  // no error message here because it's assumed VLoadGameDelegate() kicked out the error

    // trigger the Environment Loaded Game event - only then can player actors and AI be spawned!
    SendEvent(String("Environment_Loaded"));

    return true;
}

// ----------------------------------------------------------
// Manage game views
// ----------------------------------------------------------

void BaseGameLogic::VAddView(SharedPtr<IGameView> pView, GameNodeId actorId)
{
    // This makes sure that all views have a non-zero view id.
    int viewId = static_cast<int>(m_GameViews.Size());
    m_GameViews.Push(pView);
    pView->VOnAttach(viewId, actorId);
    if (!pView->VOnRestore())
    {
        URHO3D_LOGERROR("Failed to initialize game view");
    }
}

void BaseGameLogic::VRemoveView(SharedPtr<IGameView> pView)
{
    GameViewList::Iterator it = m_GameViews.Find(pView);
    if (it->NotNull())
    {
        m_GameViews.Erase(it);
    }
}

void BaseGameLogic::InitializeComponents()
{

}


// ----------------------------------------------------------
// Delegates routines
// ----------------------------------------------------------

void BaseGameLogic::InitializeAllDelegates()
{
    SubscribeToEvent("Request_Destroy_Actor", URHO3D_HANDLER(BaseGameLogic, RequestDestroyActorDelegate));
    SubscribeToEvent("Request_Start_Game", URHO3D_HANDLER(BaseGameLogic, RequestStartGameDelegate));
    SubscribeToEvent("Environment_Loaded", URHO3D_HANDLER(BaseGameLogic, EnvironmentLoadedDelegate));
}

void BaseGameLogic::DestroyAllDelegates()
{
    UnsubscribeFromAllEvents();
}


void BaseGameLogic::RequestDestroyActorDelegate(StringHash eventType, VariantMap& eventData)
{
    GameNodeId actorId = eventData["ActorId"].Get<GameNodeId>();
    VDestroyGameNode(actorId);
}

void BaseGameLogic::RequestStartGameDelegate(StringHash eventType, VariantMap& eventData)
{
    VChangeState(BGS_WaitingForPlayers);
}

void BaseGameLogic::EnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData)
{
    m_HumanGamesLoaded++;
}

