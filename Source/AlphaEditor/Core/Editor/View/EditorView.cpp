#include "AlphaEditorStd.h"

// Core data
#include "../Editor.h"
#include "../EditorData.h"



#include "EditorView.h"


using namespace Urho3D;

EditorView::EditorView(Context* context) : Object(context)
{
    // Updated for EngineSTD resources
    m_pCache = g_pApp->GetConstantResCache();
    m_pGraphics = g_pApp->GetGraphics();
    m_pUI = g_pApp->GetSubsystem<UI>();
}

EditorView::~EditorView()
{
    DestroyAllDelegates();
}

void EditorView::RegisterObject(Context* context)
{
    context->RegisterFactory<EditorView>();
}

bool EditorView::Initialize(Editor* editor, EditorData* data)
{
    if (!data)
    {
        return false;
    }

    m_pEditorData = data;

    XMLFile* styleFile = NULL;
    XMLFile* iconstyle = NULL;

    if (data)
    {
        styleFile = data->GetEditorDefaultStyle();
        iconstyle = data->GetEditorIconStyle();
    }

    if (!styleFile && !iconstyle)
    {
        return false;
    }

    // Set initial view width and height - Needed to set the initial width of the root
    int initialWidth = m_pGraphics->GetWidth();
    int initialHeight = m_pGraphics->GetHeight();

    // Get size
    m_pUIRoot = data->GetEditorRootUI();

    m_pUIRoot->SetMinWidth(initialWidth);
    m_pUIRoot->SetMinHeight(initialHeight);

    m_pUIRoot->SetHeight(initialWidth);
    m_pUIRoot->SetWidth(initialHeight);

    m_MinLeftFrameWidth = 20;
    m_MinRightFrameWidth = 20;
    m_MaxLeftFrameWidth = 300;
    m_MaxRightFrameWidth = 300;


    // if (!CreateMenu(styleFile, iconstyle))
    //{
    //URHO3D_LOGERROR("Failed to create menu.");
    //return false;
//    }


    //  if (!CreateToolBar(styleFile, iconstyle))
//    {
    //      URHO3D_LOGERROR("Failed to create tool bar.");
    //return false;
    //}

    m_MiniToolHeight = m_pUI->GetRoot()->GetHeight() - m_pToolbar->GetMinHeight() - m_pMenubar->GetMinHeight();

    m_MiniToolHeight=0;

    if (!CreateMainFrame(styleFile, iconstyle))
    {
        URHO3D_LOGERROR("Failed to create MainFrame.");
        return false;
    }

    //  if (!CreateMiniToolBar(styleFile, iconstyle))
//    {
    //URHO3D_LOGERROR("Failed to create mini tool bar.");
    //return false;
    //}

    //if (!CreateLeftFrame(styleFile, iconstyle))
    //{
    //  URHO3D_LOGERROR("Failed to create LeftFrame.");
//        return false;
    //  }

    if (!CreateMiddleFrame(styleFile, iconstyle))
    {
        URHO3D_LOGERROR("Failed to create MiddleFrame.");
        return false;
    }

    //if (!CreateRightFrame(styleFile, iconstyle))
    //{
    //URHO3D_LOGERROR("Failed to create RightFrame.");
    //return false;
    //}

    InitializeAllDelegates();

    //minMiddleFrameWidth_ = 200; /// \todo use minMiddleFrameWidth_


    return true;
}


bool EditorView::CreateMenu(XMLFile* styleFile, XMLFile* iconstyle)
{
    m_pMenubar = MenuBarUI::Create(m_pUIRoot, "EditorMenuBar");
    if (!m_pMenubar)
    {
        return false;
    }

    return true;
}

bool EditorView::CreateToolBar(XMLFile* styleFile, XMLFile* iconstyle)
{

    m_pToolbar = ToolBarUI::Create(m_pUIRoot, "EditorToolBar", iconstyle, "EditorToolBar", 840, 40, styleFile);

    if (!m_pToolbar)
    {
        return false;
    }

    return true;
}


bool EditorView::CreateMiniToolBar(XMLFile* styleFile, XMLFile* iconstyle)
{

    m_pMiniToolbar = MiniToolBarUI::Create(m_pMainFrame, "EditorMiniToolBar", iconstyle, 28, m_MiniToolHeight, styleFile);

    if (!m_pMiniToolbar)
    {
        return false;
    }

    return true;
}


bool EditorView::CreateMainFrame(XMLFile* styleFile, XMLFile* iconstyle)
{
    m_pMainFrame = m_pUIRoot->CreateChild<UIElement>("MainFrame");

    if (!m_pMainFrame)
    {
        return false;
    }

    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetWidth();

    m_pMainFrame->SetDefaultStyle(styleFile);
    m_pMainFrame->SetFixedSize(Width, Height);

    return true;
}

bool EditorView::CreateLeftFrame(XMLFile* styleFile, XMLFile* iconstyle)
{
    m_pLeftFrame = m_pMainFrame->CreateChild<TabWindow>("LeftFrame");

    if (!m_pLeftFrame)
    {
        return false;
    }

    m_pLeftFrame->SetStyle("TabWindow");
    m_pLeftFrame->SetWidth(200);
    m_pLeftFrame->SetFixedHeight(m_MiniToolHeight);
    m_pLeftFrame->SetResizable(true);
    m_pLeftFrame->SetMinWidth(m_MinLeftFrameWidth);
    m_pLeftFrame->SetMaxWidth(m_MaxLeftFrameWidth);
    m_pLeftFrame->SetResizeBorder(IntRect(6, 6, 6, 6));
    m_pLeftFrame->SetLayoutBorder(IntRect(4, 4, 4, 4));
    m_pLeftFrame->SetPosition(m_pMiniToolbar->GetPosition() + IntVector2(m_pMiniToolbar->GetWidth(), 0));
    return true;
}

bool EditorView::CreateMiddleFrame(XMLFile* styleFile, XMLFile* iconstyle)
{
    m_pMiddleFrame = m_pMainFrame->CreateChild<TabWindow>("MiddleFrame");

    // set frame
    if (!m_pMiddleFrame)
    {
        return false;
    }


    // probably can be removed not needed
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetWidth();

    // Set position
    m_pMiddleFrame->SetPosition(0,0);
    m_pMiddleFrame->SetStyle("TabWindow");
    m_pMiddleFrame->SetHeight(Height);
    m_pMiddleFrame->SetMinWidth(Width);
    m_pMiddleFrame->SetWidth(Width);
    m_pMiddleFrame->SetLayoutBorder(IntRect(4, 4, 4, 4));

    return true;
}

bool EditorView::CreateRightFrame(XMLFile* styleFile, XMLFile* iconstyle)
{
    m_pRightFrame = m_pMainFrame->CreateChild<TabWindow>("LeftFrame");

    if (!m_pRightFrame)
    {
        return false;
    }

    m_pRightFrame->SetPosition(m_pMiddleFrame->GetPosition() + IntVector2(m_pMiddleFrame->GetWidth(), 0));
    m_pRightFrame->SetStyle("TabWindow");
    m_pRightFrame->SetLayout(LM_VERTICAL);
    m_pRightFrame->SetWidth(200);
    m_pRightFrame->SetResizable(true);
    m_pRightFrame->SetMinWidth(m_MinRightFrameWidth);
    m_pRightFrame->SetMaxWidth(m_MaxRightFrameWidth);
    m_pRightFrame->SetFixedHeight(m_MiniToolHeight);
    m_pRightFrame->SetResizeBorder(IntRect(6, 6, 6, 6));
    m_pRightFrame->SetLayoutBorder(IntRect(4, 4, 4, 4));
    return true;
}

void EditorView::InitializeAllDelegates()
{
    if (m_pRightFrame)
    {
        SubscribeToEvent(m_pRightFrame, E_RESIZED, URHO3D_HANDLER(EditorView, HandleResizeEventDelegate));
    }

    if (m_pLeftFrame)
    {
        SubscribeToEvent(m_pLeftFrame, E_RESIZED, URHO3D_HANDLER(EditorView, HandleResizeEventDelegate));
    }

    SubscribeToEvent(E_SCREENMODE, URHO3D_HANDLER(EditorView, HandleScreenUpdateDelegate));
}

void EditorView::DestroyAllDelegates()
{
    UnsubscribeFromAllEvents();
}


void EditorView::HandleResizeEventDelegate(StringHash eventType, VariantMap& eventData)
{
    // Catch all function now
    HandleScreenUpdateDelegate(eventType, eventData);
}

void EditorView::HandleScreenUpdateDelegate(StringHash eventType, VariantMap& eventData)
{
    UIElement* element = (UIElement*)eventData[Resized::P_ELEMENT].GetPtr();
    int newWidth = eventData[Resized::P_WIDTH].GetInt();

    int windowWidth = m_pGraphics->GetWidth();
    int windowHeight = m_pGraphics->GetHeight();

    // Reset minimum height and width
    m_pUIRoot->SetMinWidth(windowWidth);
    m_pUIRoot->SetMinHeight(windowHeight);

    m_pUIRoot->SetHeight(windowWidth);
    m_pUIRoot->SetWidth(windowHeight);

    // Work on the rest next
    //m_MiniToolHeight = windowHeight - m_pMenubar->GetMinHeight();

    // Change fixed size
    m_pMainFrame->SetFixedSize(windowWidth, windowHeight);
    m_pMiddleFrame->SetFixedSize(windowWidth, windowHeight);


    // Store height and width
    m_PreviousHeight= windowHeight;
    m_PreviousWidth = windowWidth;

    return;
}


void EditorView::UpdateLayout()
{

}

