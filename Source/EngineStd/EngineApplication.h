#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

//----------------------------------------------------------------------------------------------------
// EngineApp - Application class contains all necessary components EngineStd.hin order to run program correctly.
//----------------------------------------------------------------------------------------------------

#include "EnginePlatform.h"

#include "Mainloop/Initialization.h"

using namespace Urho3D;

class BaseGameLogic;

// Network
class BaseSocketManager;
class NetworkEventForwarder;

class EngineApp : public Application
{
public:
	// Constructors
	EngineApp(Context* context);
	virtual ~EngineApp();

	// Application class overrided functions
	virtual void Setup();						// Called before engine initialization. Logging system and
												// user specific game options init here

	virtual void Start();						// Called after engine initialization. Setup application & subscribe to events here
	virtual void Stop();						// Perform optional cleanup after main loop has terminated


	void AbortGame();

	// Game specific. Entry point to load the game.
	// It can be different loading steps for one game depend on platform. (pc, mobile, ...)
	bool VLoadGame();

	// Network
	bool AttachAsClient();

protected:
	// Initialize instance. Set startup parameters for window and
	// other subsystems
	void InitInstance(int screenWidth, int screenHeight, bool windowMode, bool vsync, int multisample, bool triplebuffer);

	// Cursor routine
	virtual bool CreateCursor();				// Create all cursor shapes from XML file.

	// Create game logic and game views
	bool virtual VCreateViewLogic() = 0;			// Create specific game logic and game view.

	virtual void VInitializeAllDelegates();				// Register all delegates
	virtual void VDestroyAllDelegates();					// Unsubscribe from all events

	// Delegates
	void UpdateDelegate(StringHash eventType, VariantMap& eventData);		// Update delegeta to send update messsage
																			// to game logic
	// Message handling
	bool OnMessageProc(AppMsg msg);

private:
	// Utility function
	void CreateConsole(XMLFile* style);
	void CreateDebugHud(XMLFile* style);

	// Input/output delegates
	void KeyDownDelegate(StringHash eventType, VariantMap& eventData);		// Handle keyboarad's button down event
	void KeyUpDelegate(StringHash eventType, VariantMap& eventData);		// Handle keyboarad's button down event
	void MouseMoveDelegate(StringHash eventType, VariantMap& eventData);	// Handle mouse's event
	void MouseDownDelegate(StringHash eventType, VariantMap& eventData);	// Handle mouse's event
	void MouseUpDelegate(StringHash eventType, VariantMap& eventData);		// Handle mouse's event

	// Window delegates
	void WindowDelegate(StringHash eventType, VariantMap& eventData);		// Handle window's mode
	void InputFocusDelegate(StringHash eventType, VariantMap& eventData);	// Handle when focus is lost (e.x. window minimized)
	void SetWindowSize(IntVector2 size) { m_CurrentWindowSize.x_ = size.x_; m_CurrentWindowSize.y_ = size.y_; }

public:
	// Getter/Setters
	inline SharedPtr<Engine> GetEngine() { return engine_;  }
	inline Renderer* GetRenderer() { return m_pRenderer; }
	inline Graphics* GetGraphics() { return m_pGraphics; }
	inline Audio* GetAudio() { return m_pAudio; }
	inline FileSystem * GetFileSystem() {return m_pFileSystem;}
	inline ResourceCache* GetConstantResCache() { return m_pConstantResourceCache; }
	inline BaseGameLogic* GetGameLogic() const { return m_pGameLogic; }
	inline Time* GetTimer() { return m_pTime; }

	inline Network* GetNetwork() { return m_pNetwork; }
	inline BaseSocketManager* GetSocketManager() { return m_pBaseSocketManager; }
	inline void SetSocketManager(BaseSocketManager* manager) { m_pBaseSocketManager = manager; }
	inline NetworkEventForwarder* GetEventForwarder() { return m_pNetworkEventer;  }


	// Game specific getters/setters
	virtual String GetWindowIcon() = 0;
	virtual String GetWindowTitle() = 0;
	inline bool GetWindowMode() { return m_bIsWindowMode; }

	inline String GetCurrentWorkDirectory() { return m_CurrentWorkDirectory; }
	inline IntVector2 GetCurrentWindowSize() { return m_CurrentWindowSize; }
	inline String GetSaveDirectory() { return m_SaveDirectory; }
	inline GameOptions& GetGameOptions() { return m_GameOptions;  }
	inline PlatformOS GetCurrentPlatform() { return m_CurrentPlatform;}

    // Get OS
    PlatformOS GetOS(String InputString);

protected:
// For caching
	Graphics* m_pGraphics;						// Application window handling and parameters.
	Renderer* m_pRenderer;						// 3D rendering capabilities, scene rendering.
	Audio* m_pAudio;							// Audio system, only for UI element.

	UI* m_pUI;									// Graphical user interface elements

	ResourceCache* m_pConstantResourceCache;	// Resource cache for constrant resources
	FileSystem* m_pFileSystem;					// File system access

	Input* m_pInput;							// Input device access (can choose poll or event-based mode)

	Time* m_pTime;								// Manage frame updates, frame number and elapsed time countig
												// and controls the frequency of the OS low-resolution timer

	WorkQueue* m_pWorkQueue;					// Executes background tasks in worker threads

// Network

	Network* m_pNetwork;
	BaseSocketManager* m_pBaseSocketManager;    // Can be server/client
	NetworkEventForwarder* m_pNetworkEventer;	// Read events from server

// Game specific
	Cursor* m_pCurrentCursor;					// Current cursor, that handle UI system.

	GameOptions m_GameOptions;					// User specific startup game options
	BaseGameLogic* m_pGameLogic;				// Game logic

	String m_CurrentWorkDirectory;
	String m_SaveDirectory;

	IntVector2 m_CurrentWindowSize;
	bool m_bIsWindowMode;						// True - window mode;

	String m_Renderer;							// What renderer API we use. DX9, D11, OpenGL

	Vector2 m_LastMousePos;
	bool m_bIsInit;								// True - already initialized all subsystems. Game can be started.

	PlatformOS m_CurrentPlatform;               // Current platform

};

extern EngineApp* g_pApp;						// Global pointer, which is visible for whole program.

#endif // ENGINE_APPLICATION_H
