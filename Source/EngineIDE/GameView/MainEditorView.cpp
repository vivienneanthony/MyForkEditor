//
// EngineSTD - EngineIDE Development Interface
//
// Programmers
// Viviennie Anthony
//
// Vivienne Anthony - Copyright (c) 2008-2015
//
#include "EngineIDEStd.h"


// EngineStd Additional Headers
#include "EngineStd/Mainloop/Activity/ActivityManager.h"
#include "EngineStd/GameLogic/BaseGameLogic.h"
#include "EngineStd/EventManager/Local/LocalEvents.h"

//#include "../UserInterface/MainMenuUI.h"
#include "../Activities/Intro/IntroActivity.h"

// Additonal headers
#include "../Core/Editor/Editor.h"

// Main Menu View Header
#include "MainEditorView.h"

using namespace Urho3D;

MainEditorView::MainEditorView(Context* context, Renderer* renderer, bool intro) : HumanView(context, renderer)
    ,m_pEditor(NULL)
{

    // Initialize
    VInitializeAllDelegates();

    if (intro)
    {
        StrongActivityPtr intro = SharedPtr<Activity>(new IntroActivity(context_));
        m_pActivityManager->AttachActivity(intro);
    }
    else
    {
        SendEvent(Event_Data_Finish_Introduction_Part::g_EventType);
    }
}

MainEditorView::~MainEditorView()
{
    SAFE_DELETE(m_pEditor);

    return;
}

void  MainEditorView::VInitializeAllDelegates()
{
    HumanView::VInitializeAllDelegates();
    SubscribeToEvent(Event_Data_Finish_Introduction_Part::g_EventType, URHO3D_HANDLER(MainEditorView, FinishIntroductionPartDelegate));

    return;
}

void  MainEditorView::VDestroyAllDelegates()
{
    HumanView::VDestroyAllDelegates();

    return;
}

void  MainEditorView::FinishIntroductionPartDelegate(StringHash eventType, VariantMap& eventData)
{
    // Unsubscribe
    UnsubscribeFromEvent(Event_Data_Finish_Introduction_Part::g_EventType);

    // Stare a new editor Instance
    EditorInstance();

    return;
}

void MainEditorView::EditorInstance()
{
    ResourceCache *ResCache = g_pApp->GetConstantResCache();
    Input* p_InputSystem = g_pApp->GetSubsystem<Input>();

    // Create a Editor instant
    m_pEditor = new Editor(g_pApp->GetGameLogic()->GetContext());

    // Initialize cursor system
    p_InputSystem->SetMouseVisible(true);

    // Create console and debug
    CreateConsoleAndDebugHud();

    // Get current UI
    UI* pUI = g_pApp->GetSubsystem<UI>();

    // Get Root
    UIElement* pUIRoot = pUI->GetRoot();

    XMLFile* xmlFile = ResCache->GetResource<XMLFile>("UI/IDEStyle.xml");

    // Create a root for IDE - Crashes without it
    m_pUIRootElement = pUIRoot->CreateChild<UIElement>("IDERoot");
    m_pUIRootElement->SetTraversalMode(TM_DEPTH_FIRST);         // This is needed for root-like element to prevent artifacts
    m_pUIRootElement->SetDefaultStyle(xmlFile);

    // Initialize
    m_pEditor->Initialize();

    // Create the Editor
    m_pEditor->Create(m_pScene, m_pUIRootElement);

    // Plugins
    m_pEditor->LoadPlugins();

    // Subscribe to events
    SubscribeToEvent(E_EDITORINSTANCE, URHO3D_HANDLER(MainEditorView, HandleEditorInstanceClose));

    return;
}
// Handle creating console
void MainEditorView::CreateConsoleAndDebugHud(void)
{
    ResourceCache *ResCache = g_pApp->GetConstantResCache();

    XMLFile* xmlFile = ResCache->GetResource<XMLFile>("UI/IDEStyle.xml"); // Changed default style to IDStyle.xml

    // Verify ResCache and XMLFile exist
    if(ResCache&&xmlFile)
    {
        // Create console
        m_pConsole = g_pApp->GetEngine()->CreateConsole();
        m_pConsole->SetDefaultStyle(xmlFile);
        m_pConsole->SetVisible(false);

        // Create debug HUD.
        m_pDebugHud =g_pApp->GetEngine()->CreateDebugHud();
        m_pDebugHud->SetDefaultStyle(xmlFile);

        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MainEditorView, HandleKeyDown));
    }
    else
    {
        URHO3D_LOGERROR("Could not create DebugHUD and Console.");
    }

    return;
}

// Handle closing
void MainEditorView::HandleEditorInstanceClose(StringHash eventType, VariantMap& eventData)
{
    unsigned int action = eventData[EditorInstanceEvent::P_ACTION].GetFloat();

    // if action is quit then abort
    if(action == -1)
    {
        // abort game exit
        g_pApp->AbortGame();
    }

    return;
}

void MainEditorView::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
    using namespace KeyDown;

    int key = eventData[P_KEY].GetInt();

    // Toggle console with F1
    if (key == KEY_F1)
    {
        m_pConsole->Toggle();
    }
    else if (key == KEY_F2)
    {

        m_pDebugHud->Toggle(DEBUGHUD_SHOW_PROFILER);
        m_pConsole->SetVisible(false);
    }

    return;
}
