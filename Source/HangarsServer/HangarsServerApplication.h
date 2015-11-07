#ifndef HANGARS_SERVER_APPLICATION_H
#define HANGARS_SERVER_APPLICATION_H

class HangarsServerApp : public EngineApp
{
public:
	// Constructors
	HangarsServerApp(Context* context);
	virtual ~HangarsServerApp();

public:
	// Application class overrided functions
	virtual void Setup();

	virtual void Start();						// Called after engine initialization. Setup application & subscribe to events here
	virtual void Stop();						// Perform optional cleanup after main loop has terminated

	virtual bool CreateCursor();

	// Create game logic and game views
	virtual bool VCreateViewLogic();			// Create Hanagrs game logic

	virtual String GetWindowIcon() { return "System/Hangars.jpg"; }
	virtual String GetWindowTitle() { return "HangarsServerApp"; }

	virtual void InitializeAllDelegates();				// Register all delegates
	virtual void DestroyAllDelegates();					// Unsubscribe from all events to game logic
};

#endif //HANGARS_SERVER_APPLICATION_H
