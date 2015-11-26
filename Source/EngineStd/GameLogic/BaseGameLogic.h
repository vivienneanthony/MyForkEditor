#ifndef BASE_GAME_LOGIC_H
#define BASE_GAME_LOGIC_H

class ActivityManager;
class AwesomiumCore;
class LevelManager;
class GameAssetFactory;
class GameAssetManager;

enum BaseGameState : int
{
	BGS_Invalid = 0,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_WaitingForPlayers,
	BGS_LoadingGameEnvironment,
	BGS_WaitingForPlayersToLoadEnvironment,
	BGS_SpawningPlayersActors,
	BGS_Running
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
	virtual void VDestroyGameNode(const GameNodeId gameNodeId);

	// Handle game state
	virtual void VChangeState(enum BaseGameState newState);
	virtual bool VLoadGame(String levelResource);

	// Manage game views
	virtual void VAddView(SharedPtr<IGameView> pView, GameNodeId gameNodeId = INVALID_GAME_NODE_ID);
	virtual void VRemoveView(SharedPtr<IGameView> pView);

	// Getter/Setters
	inline ActivityManager* GetActivityManager() { return m_pActivityManager; }
	inline GameViewList& GetGameViews() { return m_GameViews; }
	inline LevelManager* GetLevelManager() { return m_pLevelManager; }
	inline SharedPtr<Scene> GetScene() { return m_pScene; }

    // Game Assets Getters/Setters
    inline GameAssetManager * GetGameAssetManager()  {return m_pGameAssetManager;}
	inline GameAssetFactory * GetGameAssetFactory()  {return m_pGameAssetFactory;}

protected:
	// Override this function to do any game-specific loading.
	virtual bool VLoadGameDelegate(String pLevelData) { return true; }

private:
	virtual void InitializeComponents();		// Register factory and attributes for the component so it can be created via CreateComponent, and loaded / saved

private:
	virtual void InitializeAllDelegates();		// Register all delegates
	virtual void DestroyAllDelegates();			// Unsubscribe from all events

	// Delegates
	void RequestDestroyActorDelegate(StringHash eventType, VariantMap& eventData);
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
};

#endif // BASE_GAME_LOGIC_H
