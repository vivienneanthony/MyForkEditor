#ifndef HANGARS_CLIENT_APPLICATION_H
#define HANGARS_CLIENT_APPLICATION_H

class HangarsClientApp : public EngineApp
{
	URHO3D_OBJECT(HangarsClientApp, EngineApp)
public:
	// Constructors
	HangarsClientApp(Context* context);
	virtual ~HangarsClientApp();

public:
	// Application class overrided functions
	virtual void Setup();

	virtual void Start();						// Called after engine initialization. Setup application & subscribe to events here
	virtual void Stop();						// Perform optional cleanup after main loop has terminated

	virtual bool CreateCursor();

	// Create game logic and game views
	virtual bool VCreateViewLogic();			// Create Hanagrs game logic

	virtual String GetWindowIcon() { return "System/Hangars.jpg"; }
	virtual String GetWindowTitle() { return "Hangars"; }

	virtual void InitializeAllDelegates();				// Register all delegates
	virtual void DestroyAllDelegates();					// Unsubscribe from all events to game logic
};


#endif //HANGARS_CLIENT_APPLICATION_H
