#ifndef BASE_GAME_LOGIC_H
#define BASE_GAME_LOGIC_H

class ActivityManager;
class AwesomiumCore;
class LevelManager;
class GameAsset;
class GameAssetFactory;
class GameAssetManager;

enum BaseGameState : int
{
	// Common client/server states
	BGS_Invalid = 0,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_Running,

	// Client game states
	BGS_WaitingForPlayer,
	BGS_LoadingPlayerLobby,
	BGS_LoadingGameEnvironment,
	BGS_SpawningPlayerGameNode,
	
	// Server game states
	BGS_WaitingForServerCreating,
	BGS_WaitingForServerRestarting,
	BGS_WaitingForServerPause,
	BGS_WaitingForServerStop,
	BGS_ServerCreated,
	BGS_ServerRestarted,
	BGS_ServerPaused,
	BGS_ServerStopped,
};

class BaseGameLogic : public IGameLogic
{
	// This is neccessary, in order that get access to GameViews.
	friend class EngineApp;


	URHO3D_OBJECT(BaseGameLogic, Object)
public:
	// Constructors
	BaseGameLogic(Context *context);
	virtual ~BaseGameLogic();

	// Init, run, shutdown
	virtual bool VInitialize();
	virtual void VOnUpdate(float timeStep);
	virtual void VShutdown();

	// Manipulation with actors
	virtual StrongNodePtr VCreateGameNode(const GameAsset* gameAsset, const Matrix4* initialTransform = NULL, const GameNodeId serversGameNodeId = INVALID_GAME_NODE_ID);
	virtual StrongNodePtr VCreateGameNode(const String gameAssetName, const Matrix4* initialTransform = NULL, const GameNodeId serversGameNodeId = INVALID_GAME_NODE_ID);

	
	virtual WeakNodePtr VGetGameNode(const GameNodeId gameNodeId);
	virtual void VDestroyGameNode(const GameNodeId gameNodeId);

	// Handle game state
	virtual void VChangeState(enum BaseGameState newState);
	virtual bool VLoadGame(String levelResource);

	// Manage game views
	virtual void VAddView(SharedPtr<IGameView> pView, GameNodeId gameNodeId = INVALID_GAME_NODE_ID);
	virtual void VRemoveView(SharedPtr<IGameView> pView);

	// Network
	void SetLoginSuccess(bool success, String reason = String::EMPTY); 
	
	// Getter/Setters
	inline ActivityManager* GetActivityManager() { return m_pActivityManager; }
	inline GameViewList& GetGameViews() { return m_GameViews; }
	inline LevelManager* GetLevelManager() { return m_pLevelManager; }
	inline SharedPtr<Scene> GetScene() { return m_pScene; }

	inline void SetServerCreated(bool value) { m_bIsServerCreated = value; }
	inline bool IsServerCreated() { return m_bIsServerCreated; }

	inline bool IsProxy() { return m_bIsProxy; }

    // Game Assets Getters/Setters
    inline GameAssetManager *GetGameAssetManager()  {return m_pGameAssetManager;}
	inline GameAssetFactory *GetGameAssetFactory()  {return m_pGameAssetFactory;}

protected:
	// Network
	virtual void VSetProxy();

	// Override this function to do any game-specific loading.
	virtual bool VLoadGameDelegate(String pLevelData) { return true; }

protected:
	virtual void VInitializeAllDelegates();		// Register all delegates
	virtual void VDestroyAllDelegates();			// Unsubscribe from all events

	// Delegates
	void RequestNewGameNodeDelegate(StringHash eventType, VariantMap& eventData);
	void RequestDestroyGameNodeDelegate(StringHash eventType, VariantMap& eventData);
	
	// Game events delegates
	void RequestStartGameDelegate(StringHash eventType, VariantMap& eventData);
	void EnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData);

protected:
	float m_Lifetime;									// Indicates how long this game has been in session

	ActivityManager* m_pActivityManager;				// A game logic entity

	BaseGameState m_State;								// Game state: loading, running, etc.
	GameViewList m_GameViews;							// Views that are attached to our game

	SharedPtr<Scene> m_pScene;							// Game scene

	LevelManager* m_pLevelManager;						// Manages loading and chaining levels

	GameAssetManager * m_pGameAssetManager;             // Manages and loads game assets data
	GameAssetFactory* m_pGameAssetFactory;				// Game Asset Factory. GameAsset -> Node.

	bool m_bIsRenderDiagnostic;							// Are we rendering diagnostics?

	int m_HumanGamesLoaded;
	int m_ExpectedPlayers;								// How many players will play with us ?
	int m_HumanPlayersAttached;							// How many players actually play with us

	bool m_bIsProxy;									// Is it client(proxy) connection, not real one.
	bool m_bIsServerCreated;							// For server side.
	bool m_bIsPlayerLoggedIn;							// TRUE - If password/login was corrected, FALSE - contrary
};

#endif // BASE_GAME_LOGIC_H
