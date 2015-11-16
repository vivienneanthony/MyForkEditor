#include "EngineStd.h"
#include "Debugging/Logger.h"

#include "GameLogic/BaseGameLogic.h"

#include "Interfaces/IGameView.h"

#include "EnginePlatform.h"

#include "EngineApplication.h"


EngineApp* g_pApp = nullptr;

// Application entry-point


EngineApp::EngineApp(Context* context) : Application(context)
{
    g_pApp = this;
    m_bIsInit = false;
    m_pCurrentCursor = nullptr;
}

EngineApp::~EngineApp()
{

}


void EngineApp::Setup()
{
    m_bIsInit = false;
    // Initialize logging system
    Logger::Init("logging.log");

    // Get this platform
    m_CurrentPlatform = GetOS(GetPlatform());
	URHO3D_LOGDEBUG(String("Current platform is ") + GetPlatform());

    // Set Platform
    m_GameOptions.SetPlatform(m_CurrentPlatform);

    m_GameOptions.InitRenderer();

    // set filename based on platform
    String PlayerOptionFile;

    if(m_CurrentPlatform != PlatformWindows)
    {
        PlayerOptionFile.Append("System/GameOptions.xml");
    }
    else
    {
        PlayerOptionFile.Append("System\\GameOptions.xml");
    }

    if (!m_GameOptions.InitGameOptions(PlayerOptionFile.CString()))
    {
        URHO3D_LOGERROR("Failed to load game options!");
    }

    m_CurrentWindowSize.x_ = m_GameOptions.m_ScreenSize.x_;
    m_CurrentWindowSize.y_ = m_GameOptions.m_ScreenSize.y_;

    InitInstance(static_cast<int>(m_GameOptions.m_ScreenSize.x_),
                 static_cast<int>(m_GameOptions.m_ScreenSize.y_),
                 m_GameOptions.m_bWindowMode,
                 m_GameOptions.m_bVSync,
                 m_GameOptions.m_Multisample,
                 m_GameOptions.m_bTripleBuffer
                );
}


void EngineApp::Start()
{
    // Save all necessary subsystems in Game Application Layer
    context_->RegisterSubsystem(new Script(context_));

    // Add resource manager
    GameAssetManager::RegisterNewSubsystem(context_);

    /*context_->RegisterSubsystem(new Audio(context_));
    */

    m_pGraphics = GetSubsystem<Graphics>();
    m_pRenderer = GetSubsystem<Renderer>();
    m_pUI = GetSubsystem<UI>();
    m_pAudio = GetSubsystem<Audio>();

    m_pConstantResourceCache = GetSubsystem<ResourceCache>();
    m_pFileSystem = GetSubsystem<FileSystem>();

    m_pInput = GetSubsystem<Input>();
    m_pTime = GetSubsystem<Time>();
    m_pWorkQueue = GetSubsystem<WorkQueue>();

    m_CurrentWorkDirectory = "";
    m_CurrentWorkDirectory = m_pFileSystem->GetProgramDir();

    m_SaveDirectory = "";
    m_SaveDirectory = m_CurrentWorkDirectory;
    m_SaveDirectory += "GameData/Save";

    VInitializeAllDelegates();


    if (CreateCursor())
    {
        m_pInput->SetMouseVisible(true);
        m_pCurrentCursor->SetVisible(true);
    }
    else
    {
        URHO3D_LOGERROR(String("Failed to init game cursors"));
    }

    // Load XML file containing default UI style sheet
    XMLFile* style = m_pConstantResourceCache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(style);

    if (!VCreateViewLogic())
    {
        URHO3D_LOGERROR(String("Failed to init game logic"));
        ErrorDialog("Error", "Failed to init game logic");
        AbortGame();
    }

    m_pAudio->SetMasterGain(SOUND_MASTER, m_GameOptions.m_MasterVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_EFFECT, m_GameOptions.m_SoundEffectsVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_MUSIC, m_GameOptions.m_MusicVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_VOICE, m_GameOptions.m_SpeechVolume / 100.0f);


	CreateConsole(style);

	CreateDebugHud(style);

  
    m_bIsInit = true;
    URHO3D_LOGINFO("Game can be started");
}

void EngineApp::Stop()
{
    m_pGameLogic->VShutdown();

    VDestroyAllDelegates();

    Logger::Shutdown();
}

void EngineApp::AbortGame()
{
    engine_->Exit();

}

void EngineApp::InitInstance(int screenWidth, int screenHeight, bool windowMode, bool vsync, int multisample, bool triplebuffer)
{

    m_bIsWindowMode = windowMode;

    // engineParameters_ member variable can be modified here
    if (windowMode)
    {
        engineParameters_["WindowWidth"] = screenWidth;
        engineParameters_["WindowHeight"] = screenHeight;
    }

	if (m_GameOptions.m_bUseDevelopmentDirectories)
	{
		engineParameters_["ResourcePaths"] = String("Data;CoreData;GameData");
	}
	else
	{
		engineParameters_["ResourcePackages"] = String("CoreData.pak;Data.pak;GameData.pak");
		engineParameters_["ResourcePaths"] = String("CoreData;Data;GameData");
	}

    engineParameters_["FullScreen"] = !windowMode;
    engineParameters_["VSync"] = vsync;
    engineParameters_["TripleBuffer"] = triplebuffer;
    engineParameters_["Multisample"] = multisample;
    engineParameters_["WindowIcon"] = GetWindowIcon();
    engineParameters_["WindowTitle"] = GetWindowTitle();
    engineParameters_["WindowResizable"] = true;
    engineParameters_["LogName"] = "";
}

bool EngineApp::CreateCursor()
{
    Cursor* cursor = new Cursor(context_);
    XMLFile* styles = m_pConstantResourceCache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    cursor->SetDefaultStyle(styles);
    cursor->SetStyleAuto(styles);

    m_pUI->SetCursor(cursor);

    m_pCurrentCursor = cursor;
    return true;
}

bool EngineApp::VLoadGame()
{
    // Read the game options and see what the current game
    // needs to be - all of the game graphics are initialized by now, too...
    return m_pGameLogic->VLoadGame(m_GameOptions.m_Level);
}


bool EngineApp::OnMessageProc(AppMsg message)
{
    bool result = false;
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly


    switch (message.uEvent)
    {
    case APP_EVENT::KEY_DOWN:
    case APP_EVENT::KEY_UP:
    case APP_EVENT::MOUSE_MOVE:
    case APP_EVENT::MOUSE_LBUTTONDOWN:
    case APP_EVENT::MOUSE_LBUTTONUP:
    case APP_EVENT::MOUSE_RBUTTONDOWN:
    case APP_EVENT::MOUSE_RBUTTONUP:
    {
        GameViewList& gameViews = m_pGameLogic->GetGameViews();

        for (GameViewList::Iterator i = gameViews.Begin(); i != gameViews.End(); ++i)
        {
            if (i->NotNull())
            {
                if ((*i)->VOnMsgProc(message))
                {
                    return true;
                }
            }
        }
    }
    break;
    }

    return result;
}

void EngineApp::CreateConsole(XMLFile* style)
{
	// Create console
	context_->RegisterSubsystem(new Console(context_));
	
	// Show the console by default, make it large. Console will show the text edit field when there is at least one
	// subscriber for the console command event	
	Console* console = GetSubsystem<Console>();
	console->SetNumRows(GetSubsystem<Graphics>()->GetHeight() / 16);
	console->SetNumBufferedRows(2 * console->GetNumRows());
	console->SetCommandInterpreter(GetTypeName());
	console->SetVisible(false);
	console->GetCloseButton()->SetVisible(true);
	console->SetDefaultStyle(style);
	console->GetBackground()->SetOpacity(0.8f);
	// Open the operating system console window (for stdin / stdout) if not open yet
	OpenConsoleWindow();
}

void EngineApp::CreateDebugHud(XMLFile* style)
{
	// Create debug HUD.
	DebugHud* debugHud = engine_->CreateDebugHud();
	debugHud->SetDefaultStyle(style);
}

void EngineApp::VInitializeAllDelegates()
{
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(EngineApp, UpdateDelegate));
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(EngineApp, KeyDownDelegate));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(EngineApp, KeyUpDelegate));
    SubscribeToEvent(E_MOUSEMOVE, URHO3D_HANDLER(EngineApp, MouseMoveDelegate));
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(EngineApp, MouseDownDelegate));
    SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(EngineApp, MouseUpDelegate));
    SubscribeToEvent(E_SCREENMODE, URHO3D_HANDLER(EngineApp, WindowDelegate));
    SubscribeToEvent(E_INPUTFOCUS, URHO3D_HANDLER(EngineApp, InputFocusDelegate));
}

void EngineApp::VDestroyAllDelegates()
{
    UnsubscribeFromAllEvents();
}

// -----------------------------------------
// Application delegates
// -----------------------------------------

void EngineApp::UpdateDelegate(StringHash eventType, VariantMap& eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();



    m_pGameLogic->VOnUpdate(timeStep);
}

void EngineApp::KeyDownDelegate(StringHash eventType, VariantMap& eventData)
{
    using namespace KeyDown;
    // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
    int key = eventData[P_KEY].GetInt();

    if (key == KEY_F2)
    {
        if (GetSubsystem<Console>()->IsVisible())
        {
            GetSubsystem<Console>()->SetVisible(false);
        }
        else
        {
            GetSubsystem<Console>()->SetVisible(true);
        }
    }

    if (key == KEY_ESC)
    {
        AbortGame();
    }

    AppMsg msg;
    msg.uEvent = APP_EVENT::KEY_DOWN;
    msg.lParam.m_keycode = key;
    OnMessageProc(msg);

}

void EngineApp::MouseMoveDelegate(StringHash eventType, VariantMap& eventData)
{
    AppMsg msg;
    msg.uEvent = MOUSE_MOVE;
    msg.lParam.m_position.x_ = static_cast<float>(eventData[Urho3D::MouseMove::P_X].GetInt());
    msg.lParam.m_position.y_ = static_cast<float>(eventData[Urho3D::MouseMove::P_Y].GetInt());
    m_LastMousePos = Vector2(msg.lParam.m_position.x_, msg.lParam.m_position.y_);
    OnMessageProc(msg);
}

void EngineApp::MouseDownDelegate(StringHash eventType, VariantMap& eventData)
{
    AppMsg msg;
    int button = eventData[Urho3D::MouseButtonDown::P_BUTTON].GetInt();
    if (button == Urho3D::MOUSEB_LEFT)
    {
        msg.uEvent = MOUSE_LBUTTONDOWN;
    }
    else
    {
        msg.uEvent = MOUSE_RBUTTONDOWN;
    }

    msg.lParam.m_position.x_ = m_LastMousePos.x_;
    msg.lParam.m_position.y_ = m_LastMousePos.y_;

    OnMessageProc(msg);
}


void EngineApp::MouseUpDelegate(StringHash eventType, VariantMap& eventData)
{
    AppMsg msg;
    int button = eventData[Urho3D::MouseButtonUp::P_BUTTON].GetInt();
    if (button == Urho3D::MOUSEB_LEFT)
    {
        msg.uEvent = MOUSE_LBUTTONUP;
    }
    else
    {
        msg.uEvent = MOUSE_RBUTTONUP;
    }

    msg.lParam.m_position.x_ = m_LastMousePos.x_;
    msg.lParam.m_position.y_ = m_LastMousePos.y_;

    OnMessageProc(msg);
}

void EngineApp::WindowDelegate(StringHash eventType, VariantMap& eventData)
{
    Graphics* graphics = GetSubsystem<Graphics>();
    bool screenmode = m_GameOptions.m_bWindowMode;

    int width = eventData[Urho3D::ScreenMode::P_WIDTH].GetInt();
    int height = eventData[Urho3D::ScreenMode::P_HEIGHT].GetInt();

    if (g_pApp->m_bIsInit)
    {
        if (width < 800 || height < 600)
        {
            m_pGraphics->SetMode(800, 600);
            width = 800;
            height = 600;
        }

        m_CurrentWindowSize.x_ = width;
        m_CurrentWindowSize.y_ = height;
    }
}

void EngineApp::InputFocusDelegate(StringHash eventType, VariantMap& eventData)
{
    bool minimized = eventData[Urho3D::InputFocus::P_MINIMIZED].GetBool();
    if (minimized)
    {
        m_pAudio->SetMasterGain(SOUND_MASTER, 0.0f);

        if (m_pGameLogic)
        {
            for (GameViewList::Iterator i = m_pGameLogic->m_GameViews.Begin(); i != m_pGameLogic->m_GameViews.End(); ++i)
            {
                (*i)->VOnLostDevice();
            }
        }
    }
    else
    {
        if (m_pGameLogic)
        {
            for (GameViewList::Iterator i = m_pGameLogic->m_GameViews.Begin(); i != m_pGameLogic->m_GameViews.End(); ++i)
            {
                (*i)->VOnRestore();
            }
        }
        m_pAudio->SetMasterGain(SOUND_MASTER, (float)m_GameOptions.m_MasterVolume / 100.0f);
    }


}

void EngineApp::KeyUpDelegate(StringHash eventType, VariantMap& eventData)
{
    using namespace KeyDown;
    // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
    int key = eventData[P_KEY].GetInt();

    AppMsg msg;
    msg.uEvent = APP_EVENT::KEY_UP;
    msg.lParam.m_keycode = key;
    OnMessageProc(msg);
}


// get OS
PlatformOS EngineApp::GetOS(String InputString)
{
    /// create a return value;
    PlatformOS returnOS = PlatformUnknown;

    /// Get List
    Vector<String> OSList;

    /// Push OS
    OSList.Push("Unknown");
    OSList.Push("Windows");
    OSList.Push("Linux");
    OSList.Push("Mac OS X");
    OSList.Push("Android");
    OSList.Push("iOS");
    OSList.Push("Raspberry Pi");

    /// Loop through the size
    for(unsigned int i =0; i<OSList.Size(); i++)
    {
        /// Scan List
        if(OSList.At(i) == InputString)
        {
            returnOS = (PlatformOS)i;
        }
    }

    return returnOS;
};


