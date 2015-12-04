
#include "EngineStd.h"

// Network
#include "EventManager/Server/ServerEvents.h"
#include "EventManager/Local/LocalEvents.h"
#include "EventManager/Client/ClientEvents.h"

#include "UserInterface/HumanView/HumanView.h"

#include "Mainloop/Activity/ActivityManager.h"

#include "Resources/ResourceCache.h"

#include "GameAssetManager/Factory/GameAssetFactory.h"
#include "GameAssetManager/GameAssetManager.h"

#include "LevelManager/LevelManager.h"

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
	m_bIsProxy = false;

    m_HumanPlayersAttached = 0;
    m_ExpectedPlayers = 0;
    m_HumanGamesLoaded = 0;

	m_bIsServerCreated = false;
	m_bIsPlayerLoggedIn = false;

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

    
    VInitializeAllDelegates();

    return true;
}

void BaseGameLogic::VOnUpdate(float timeStep)
{
    m_Lifetime += timeStep;
    switch (m_State)
    {
    case BGS_Initializing:
    {
        VChangeState(BGS_MainMenu);
        break;
    }

    case BGS_MainMenu:
    {

        break;
    }

    case BGS_WaitingForPlayer:
    {
        if (m_bIsPlayerLoggedIn)
        {
			VChangeState(BGS_LoadingPlayerLobby);
        }
        break;
    }

	case BGS_LoadingPlayerLobby:
	{	

		break;
	}
    
    case BGS_SpawningPlayerGameNode:
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

    VDestroyAllDelegates();

    URHO3D_LOGINFO(String("Game logic life time is - ")
                   + String(m_Lifetime / 3600.0f) + String(" hours ")
                   + String(m_Lifetime / 60.0f) + String(" minutes ")
                   + String(m_Lifetime) + String(" seconds"));

    m_State = BGS_Invalid;
}



// ----------------------------------------------------------
// GameNode manipulations
// ----------------------------------------------------------

StrongNodePtr BaseGameLogic::VCreateGameNode(const GameAsset* gameAsset, const Matrix4* initialTransform, const GameNodeId serversGameNodeId)
{
	assert(m_pGameAssetFactory && m_pGameAssetManager);

	// if it is server, m_bIsProxy == false and serverGameNodeId must be INVALID_GAME_NODE_ID
	// because server generates new game node id
	if (!m_bIsProxy && serversGameNodeId != INVALID_GAME_NODE_ID)
		return StrongNodePtr();

	// if it is client, m_bIsProxy == true and serverGameNodeId must be sent from server.
	// So serversGameNodeId must not be equal INVALID_GAME_NODE_ID
	if (m_bIsProxy && serversGameNodeId == INVALID_GAME_NODE_ID)
		return StrongNodePtr();

	StrongNodePtr pGameNode = m_pGameAssetFactory->CreateNode(gameAsset, serversGameNodeId);
	if (pGameNode)
	{
		m_pScene->AddChild(pGameNode, pGameNode->GetID());

		// If it is server and game state is BGS_SpawningPlayerGameNode or BGS_Running  
		// then we have to send this event to clients
		if (!m_bIsProxy && (m_State == BGS_SpawningPlayerGameNode || m_State == BGS_Running))
		{
			SharedPtr<Event_Data_Request_New_Game_Node> pNewGameAsset(new Event_Data_Request_New_Game_Node(gameAsset->GetName(), initialTransform, pGameNode->GetID()));
			VariantMap map = pNewGameAsset->VSerialize();
			SendEvent(Event_Data_Request_New_Game_Node::g_EventType, map);
		}
	}
	return pGameNode;
}

StrongNodePtr BaseGameLogic::VCreateGameNode(const String gameAssetName, const Matrix4* initialTransform, const GameNodeId serversGameNodeId)
{
	assert(m_pGameAssetFactory && m_pGameAssetManager);

	GameAsset* pGameAsset = m_pGameAssetManager->FindGameAssetByName(gameAssetName);
	if (!pGameAsset)
	{
		URHO3D_LOGERROR("Failed to find GameAsset by name " + gameAssetName + " in BaseGameLogic's function VCreateGameNode()");
		return StrongNodePtr();
	}

	// if it is server, m_bIsProxy == false and serverGameNodeId must be INVALID_GAME_NODE_ID
	// because server generates new game node id
	if (!m_bIsProxy && serversGameNodeId != INVALID_GAME_NODE_ID)
		return StrongNodePtr();

	// if it is client, m_bIsProxy == true and serverGameNodeId must be sent from server.
	// So serversGameNodeId must not be equal INVALID_GAME_NODE_ID
	if (m_bIsProxy && serversGameNodeId == INVALID_GAME_NODE_ID)
		return StrongNodePtr();

	StrongNodePtr pGameNode = m_pGameAssetFactory->CreateNode(pGameAsset, serversGameNodeId);
	if (pGameNode)
	{
		m_pScene->AddChild(pGameNode, pGameNode->GetID());

		// If it is server and game state is BGS_SpawningPlayerGameNode or BGS_Running  
		// then we have to send this event to clients
		if (!m_bIsProxy && (m_State == BGS_SpawningPlayerGameNode || m_State == BGS_Running))
		{
			SharedPtr<Event_Data_Request_New_Game_Node> pNewGameAsset(new Event_Data_Request_New_Game_Node(pGameAsset->GetName(), initialTransform, pGameNode->GetID()));
			VariantMap map = pNewGameAsset->VSerialize();
			SendEvent(Event_Data_Request_New_Game_Node::g_EventType, map);
		}
	}

	return pGameNode;
}


WeakNodePtr BaseGameLogic::VGetGameNode(const GameNodeId gameNodeId)
{
	Node* node = m_pScene->GetNode(gameNodeId);
	return WeakNodePtr(node);
}

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
    if (newState == BGS_WaitingForPlayer)
    {
		//// Get rid of the Main Menu...
		//SharedPtr<IGameView> it;
		//it = m_GameViews.Front();
		//it->VShutdown();
		//m_GameViews.PopFront();

		//// Note: Split screen support would require this to change!
		//m_ExpectedPlayers = 1;
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
            VChangeState(BGS_SpawningPlayerGameNode);	// Game environments was loaded, we can spawn player specific game nodes.
            return;
        }
    }




    m_State = newState;
}

void BaseGameLogic::VSetProxy()
{
	m_bIsProxy = true;

	SubscribeToEvent(Event_Data_Request_New_Game_Node::g_EventType, URHO3D_HANDLER(BaseGameLogic, RequestNewGameNodeDelegate));
	
	
}

bool BaseGameLogic::VLoadGame(String levelResource)
{
    /*SharedPtr<File> file = g_pApp->GetConstantResCache()->GetFile(levelResource);
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
		*/

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

void BaseGameLogic::SetLoginSuccess(bool success, String reason)
{
	m_bIsPlayerLoggedIn = success;

	Event_Data_Player_Login_Result logResult;
	logResult.SetSuccess(success);
	logResult.SetReason(reason);
	VariantMap data = logResult.VSerialize();

	SendEvent(Event_Data_Player_Login_Result::g_EventType, data);
}

// ----------------------------------------------------------
// Delegates routines
// ----------------------------------------------------------

void BaseGameLogic::VInitializeAllDelegates()
{
    SubscribeToEvent(Event_Data_Destroy_Game_Node::g_EventType, URHO3D_HANDLER(BaseGameLogic, RequestDestroyGameNodeDelegate));
    SubscribeToEvent(Event_Data_Request_Start_Game::g_EventType, URHO3D_HANDLER(BaseGameLogic, RequestStartGameDelegate));
    //SubscribeToEvent("Environment_Loaded", URHO3D_HANDLER(BaseGameLogic, EnvironmentLoadedDelegate));
}

void BaseGameLogic::VDestroyAllDelegates()
{
	if (m_bIsProxy)
	{
		UnsubscribeFromEvent(Event_Data_Request_New_Game_Node::g_EventType);
	}


}


void BaseGameLogic::RequestDestroyGameNodeDelegate(StringHash eventType, VariantMap& eventData)
{

	
	
}

void BaseGameLogic::RequestNewGameNodeDelegate(StringHash eventType, VariantMap& eventData)
{
	// This should only happen if the game logic is a proxy, and there is a server asking us to create an game node
	assert(m_bIsProxy);

	if (!m_bIsProxy)
		return;

	Event_Data_Request_New_Game_Node dataNewGameAsset;
	dataNewGameAsset.VDeserialize(eventData);

	// Create the game node
	StrongNodePtr pGameNode = VCreateGameNode(dataNewGameAsset.GetGameAsset(), dataNewGameAsset.GetInitialTransform(), dataNewGameAsset.GetServerActorId());



}

void BaseGameLogic::RequestStartGameDelegate(StringHash eventType, VariantMap& eventData)
{
    VChangeState(BGS_WaitingForPlayer);
}

void BaseGameLogic::EnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData)
{
    m_HumanGamesLoaded++;
}

