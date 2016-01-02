#ifndef HANGARS_SERVER_APPLICATION_H
#define HANGARS_SERVER_APPLICATION_H

class BaseSocketManager;

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

protected:
	virtual bool CreateCursor();

	// Create game logic and game views
	virtual bool VCreateViewLogic();			// Create Hanagrs game logic

	virtual String GetWindowIcon() { return"System/HangarsClient.png"; }
	virtual String GetWindowTitle() { return "HangarsServerApp"; }


	virtual void VInitializeAllDelegates();				// Register all delegates
	virtual void VDestroyAllDelegates();				// Unsubscribe from all events to game logic
};

#endif //HANGARS_SERVER_APPLICATION_H
