#include "AlphaEngineStd.h"
#include "Debugging/Logger.h"


#include "Network/Managers/ClientSocketManager.h"
#include "Network/Managers/NetworkEventForwarder.h"

#include "EventManager/Server/ServerEvents.h"

#include "GameLogic/BaseGameLogic.h"
#include "Interfaces/IGameView.h"

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiSettings.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiInterface.h>


#include "AlphaEnginePlatform.h"
#include "AlphaEngineApp.h"


AlphaEngineApp* g_pApp = NULL;

// Application entry-point


AlphaEngineApp::AlphaEngineApp(Context* context) : Application(context)
{
    g_pApp = this;
    m_bIsInit = false;

    m_pCurrentCursor = NULL;

    m_pBaseSocketManager = NULL;
    m_pNetworkEventForwarder = NULL;

    m_pDBConnection = NULL;
}

AlphaEngineApp::~AlphaEngineApp()
{

}


void AlphaEngineApp::Setup()
{
    m_bIsInit = false;

    // Init Imui
    engineParameters_["EnableImGui"] = true; // if false does not create IMUI subsystem, default true.

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


void AlphaEngineApp::Start()
{
    // Save all necessary subsystems in Game Application Layer
    context_->RegisterSubsystem(new Script(context_));

    // If Enabled GUI then register interface
    if(engine_->GetParameter(engineParameters_, "EnableImGui").GetBool())
    {
        // Get the game client context
        context_->RegisterSubsystem(new ImGuiInterface(context_));
    }

    // Setup base defaults
    m_pDatabase = GetSubsystem<Database>();
    m_pGraphics = GetSubsystem<Graphics>();
    m_pRenderer = GetSubsystem<Renderer>();
    m_pUI = GetSubsystem<UI>();
    m_pAudio = GetSubsystem<Audio>();
    m_pNetwork = GetSubsystem<Network>();

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

    // Imgui Addition if Enabled attach to node
    if(engine_->GetParameter(engineParameters_, "EnableImGui").GetBool())
    {
        m_pImGuiInterface = GetSubsystem<ImGuiInterface>();
    }
    else
    {
        // null makes sure it's nothing
        m_pImGuiInterface = NULL;
    }

    // VInitialize
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

    // Configure sound
    m_pAudio->SetMasterGain(SOUND_MASTER, m_GameOptions.m_MasterVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_EFFECT, m_GameOptions.m_SoundEffectsVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_MUSIC, m_GameOptions.m_MusicVolume / 100.0f);
    m_pAudio->SetMasterGain(SOUND_VOICE, m_GameOptions.m_SpeechVolume / 100.0f);

    // Create Console UI
    CreateConsole(style);

    // Create Debug Hud UI
    CreateDebugHud(style);

    m_bIsInit = true;
    URHO3D_LOGINFO("Game can be started");

    // if engine parameter is true then setup imgui interface
    if(engine_->GetParameter(engineParameters_, "EnableImGui").GetBool())
    {
        // if ImGuiInterface Exist
        if(m_pImGuiInterface)
        {
            // create custom imgui settings.
            ImGuiSettings CustomSettings;
            CustomSettings.font("Fonts/Anonymous Pro.ttf",14, false);
            CustomSettings.font("Fonts/fontawesome-webfont.ttf", 14, true);
            CustomSettings.font("Fonts/UrhoCraz-Icons-Interface1.ttf", 48, false);

            // Icon range
            Vector<ImWchar> iconRanges;
            iconRanges.Push(0xf000);
            iconRanges.Push(0xf3ff);
            iconRanges.Push(0);

            // Basic Latin + Latin Supplement
            Vector<ImWchar> defaultranges;
            defaultranges.Push(0x0020);
            defaultranges.Push(0x00FF);
            defaultranges.Push(0);

            // Set ranges and config for other fonts
            CustomSettings.fontGlyphRanges("fontawesome-webfont", iconRanges);
            CustomSettings.fontConfig("fontawesome-webfont", true, true, true, 1, 1);

            CustomSettings.fontGlyphRanges("Anonymous Pro", defaultranges);
            CustomSettings.fontConfig("Anonymous Pro", false, false, false,3, 1);

            CustomSettings.fontGlyphRanges("UrhoCraz-Icons-Interface1", defaultranges);
            CustomSettings.fontConfig("UrhoCraz-Icons-Interface1", false, false, false,3, 1);

            m_pImGuiInterface->SetSettings(CustomSettings);
        }
    }

}

void AlphaEngineApp::Stop()
{
    // shutdown ImgUI
    ImGui::Shutdown();

    if (m_pBaseSocketManager)
    {
        m_pBaseSocketManager->Shutdown();
        SAFE_DELETE(m_pBaseSocketManager);

        SAFE_DELETE(m_pNetworkEventForwarder);
    }

    m_pGameLogic->VShutdown();

    //SAFE_DELETE(m_pGameLogic);

    VDestroyAllDelegates();



    Logger::Shutdown();
}

void AlphaEngineApp::AbortGame()
{

    engine_->Exit();

}

void AlphaEngineApp::InitInstance(int screenWidth, int screenHeight, bool windowMode, bool vsync, int multisample, bool triplebuffer)
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

bool AlphaEngineApp::CreateCursor()
{
    Cursor* cursor = new Cursor(context_);
    XMLFile* styles = m_pConstantResourceCache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    cursor->SetDefaultStyle(styles);
    cursor->SetStyleAuto(styles);

    m_pUI->SetCursor(cursor);

    m_pCurrentCursor = cursor;
    return true;
}

bool AlphaEngineApp::VLoadGame()
{
    // Read the game options and see what the current game
    // needs to be - all of the game graphics are initialized by now, too...
    return m_pGameLogic->VLoadGame(m_GameOptions.m_Level);
}


bool AlphaEngineApp::OnMessageProc(AppMsg message)
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

bool AlphaEngineApp::AttachAsClient()
{
    VariantMap identify;
    identify["CLIENT_LOGIN"] = m_GameOptions.m_Login;
    identify["CLIENT_PASSWORD"] = m_GameOptions.m_Password;

    if (m_pBaseSocketManager == NULL)
    {
        m_pBaseSocketManager = new ClientSocketManager(context_, m_GameOptions.m_GameHost, m_GameOptions.m_ListenPort, identify);
    }

    if (m_pBaseSocketManager)
    {
        ClientSocketManager* clientManager = (ClientSocketManager*)m_pBaseSocketManager;
        if (clientManager)
        {
            if (!clientManager->Connect())
            {
                URHO3D_LOGDEBUG(String("Client connection to server failed. Propably server is shutdown."));
                SAFE_DELETE(clientManager);
                return false;
            }
        }
    }

    return true;
}


void AlphaEngineApp::VCreateNetworkEventForwarder(void)
{
    if (m_pNetworkEventForwarder != NULL)
    {
        URHO3D_LOGERROR("Overwriting network event forwarder in AlphaEngineApp!");
        SAFE_DELETE(m_pNetworkEventForwarder);
    }

    m_pNetworkEventForwarder = new NetworkEventForwarder(INVALID_CONNECTION_ID);
}

void AlphaEngineApp::VDestroyNetworkEventForwarder(void)
{

}

void AlphaEngineApp::ForwardEventDelegate(StringHash eventType, VariantMap& eventData)
{
    m_pNetworkEventForwarder->ForwardEventDelegate(eventType, eventData);
}

DbConnection* AlphaEngineApp::ConnectToDB(String connectionString)
{
    m_pDBConnection = m_pDatabase->Connect(connectionString);
    if (!m_pDBConnection)
    {
        URHO3D_LOGINFO("Failed connect to database. Check connection string");
    }

    return m_pDBConnection;
}


void AlphaEngineApp::DestroyNetwork()
{
    URHO3D_LOGINFO("Network is destroyed");

    if (g_pApp->GetGameLogic()->IsProxy())
    {
        VDestroyNetworkEventForwarder();
    }
    else
    {
        if (m_pNetwork)
        {
            g_pApp->GetGameLogic()->SetServerCreated(false);
        }
    }

    m_pBaseSocketManager->Shutdown();

    SAFE_DELETE(m_pNetworkEventForwarder);
    SAFE_DELETE(m_pBaseSocketManager);
}

void AlphaEngineApp::CreateConsole(XMLFile* style)
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
    console->GetCloseButton()->SetVisible(false);
    console->SetDefaultStyle(style);
    console->GetBackground()->SetOpacity(0.8f);
    // Open the operating system console window (for stdin / stdout) if not open yet
    OpenConsoleWindow();
}

void AlphaEngineApp::CreateDebugHud(XMLFile* style)
{
    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(style);
}

void AlphaEngineApp::VInitializeAllDelegates()
{
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(AlphaEngineApp, UpdateDelegate));
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(AlphaEngineApp, KeyDownDelegate));
    SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(AlphaEngineApp, KeyUpDelegate));
    SubscribeToEvent(E_MOUSEMOVE, URHO3D_HANDLER(AlphaEngineApp, MouseMoveDelegate));
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(AlphaEngineApp, MouseDownDelegate));
    SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(AlphaEngineApp, MouseUpDelegate));
    SubscribeToEvent(E_SCREENMODE, URHO3D_HANDLER(AlphaEngineApp, WindowDelegate));
    SubscribeToEvent(E_INPUTFOCUS, URHO3D_HANDLER(AlphaEngineApp, InputFocusDelegate));
}

void AlphaEngineApp::VDestroyAllDelegates()
{
    UnsubscribeFromAllEvents();
}

// -----------------------------------------
// Application delegates
// -----------------------------------------

void AlphaEngineApp::UpdateDelegate(StringHash eventType, VariantMap& eventData)
{
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

    if (m_pGameLogic)
    {
        if (m_pBaseSocketManager)
        {
            // Send/Recieve/Delete network messages
            m_pBaseSocketManager->DoSelect();
        }

        m_pGameLogic->VOnUpdate(timeStep);
    }
}

void AlphaEngineApp::KeyDownDelegate(StringHash eventType, VariantMap& eventData)
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

void AlphaEngineApp::MouseMoveDelegate(StringHash eventType, VariantMap& eventData)
{
    AppMsg msg;
    msg.uEvent = MOUSE_MOVE;
    msg.lParam.m_position.x_ = static_cast<float>(eventData[Urho3D::MouseMove::P_X].GetInt());
    msg.lParam.m_position.y_ = static_cast<float>(eventData[Urho3D::MouseMove::P_Y].GetInt());
    m_LastMousePos = Vector2(msg.lParam.m_position.x_, msg.lParam.m_position.y_);
    OnMessageProc(msg);
}

void AlphaEngineApp::MouseDownDelegate(StringHash eventType, VariantMap& eventData)
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


void AlphaEngineApp::MouseUpDelegate(StringHash eventType, VariantMap& eventData)
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

void AlphaEngineApp::WindowDelegate(StringHash eventType, VariantMap& eventData)
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

void AlphaEngineApp::InputFocusDelegate(StringHash eventType, VariantMap& eventData)
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

void AlphaEngineApp::KeyUpDelegate(StringHash eventType, VariantMap& eventData)
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
PlatformOS AlphaEngineApp::GetOS(String InputString)
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


