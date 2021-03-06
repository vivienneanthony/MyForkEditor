#pragma once



class EngineIDEApp : public EngineApp
{
    URHO3D_OBJECT(EngineIDEApp, EngineApp)
public:
    // Constructors
    EngineIDEApp(Context* context);
    virtual ~EngineIDEApp();

public:
    // Application class overrided functions
    virtual void Setup();

    virtual void Start();								// Called after engine initialization. Setup application & subscribe to events here
    virtual void Stop();								// Perform optional cleanup after main loop has terminated


protected:
    virtual bool CreateCursor();

    // Create game logic and game viewsa
    virtual bool VCreateViewLogic();					// Create Hanagrs game logic

    virtual void RegisterAllComponents();

    virtual String GetWindowIcon()
    {
        return "System/EngineEditor.png";
    }
    virtual String GetWindowTitle()
    {
        return "Hangars";
    }

    virtual void VInitializeAllDelegates();				// Register all delegates
    virtual void VDestroyAllDelegates();				// Unsubscribe from all events to game logic

protected:
    SharedPtr<ScriptFile> m_pScriptFile;
    SharedPtr<UIElement> m_pRootUI;
    SharedPtr<UIElement> m_pWelcomeUI;
};












