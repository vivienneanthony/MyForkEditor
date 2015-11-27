#ifndef HUMAN_VIEW_H
#define HUMAN_VIEW_H


class ActivityManager;

class HumanView : public IGameView
{
	URHO3D_OBJECT(HumanView, Object)
public:
	// Constructors
	HumanView(Context *context, Renderer* renderer);
	virtual ~HumanView();

	// Attach view id and actor id. If it has controlled actor.
	// Ex. Human view must have the actor, that is controlled by player.
	// Ex. Main menu view doesn't have any actors.
	virtual void VOnAttach(GameViewId vid, GameNodeId aid);
	// Initialize humam view.
	virtual bool VOnRestore();
	// Lost device/input.
	virtual bool VOnLostDevice();
	// Update view.
	virtual void VOnUpdate(float timeStep);
	// Handle input message from devices
	virtual bool VOnMsgProc(AppMsg message);
	// Shutdown view.
	virtual void VShutdown();

	// Get type from game view. Can be different, depend on realization.
	virtual GameViewType VGetType() { return GameView_Human; }
	// Get unique id from game view.
	virtual GameViewId VGetId() const { return m_ViewId; }

	// Virtual methods to control the layering of interface elements
	virtual void VPushElement(SharedPtr<IScreenElement> pElement);
	virtual void VRemoveElement(SharedPtr<IScreenElement> pElement);

	bool LoadGame(SharedPtr<File> file, SharedPtr<Scene> level);

protected:
	virtual bool VLoadGameDelegate(SharedPtr<File> file, SharedPtr<Scene> level);


	// Getter/Setters
	inline ActivityManager* GetActivityManager() { return m_pActivityManager; }

protected:
	// Initialize audio system. Can exist ONLY 1 Audio system in the program.
	bool InitAudio();

protected:
	// Delegates
	virtual void VInitializeAllDelegates();				// Register all delegates
	virtual void VDestroyAllDelegates();					// Unsubscribe from all events

protected:
	GameViewId m_ViewId;							// Unique game view id.
	GameNodeId m_GameNodeId;						// Controlled game node id.

	ActivityManager* m_pActivityManager;			// Strictly for things like button animations, particles, etc.
													// Where we do not have rapid communication with game logic.
													// E.x. visual effect like animation, particles and so on.

	Renderer* m_pRenderer;							// 3D rendering capabilities, scene rendering.

	ScreenElementList m_ScreenElements;				// A game screen entity.

	SharedPtr<Scene> m_pScene;
	Node* m_pCameraNode;							// Camera node


	// Interface sensitive objects
	SharedPtr<IPointerHandler> m_pPointerHandler;   // Generic pointer handler
	int m_PointerRadius;
	SharedPtr<IKeyboardHandler> m_pKeyboardHandler;
};

#endif //HUMAN_VIEW_H
