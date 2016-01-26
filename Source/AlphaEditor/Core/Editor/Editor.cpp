﻿#include "AlphaEditorStd.h"

// Urho3d
#include <Urho3D/UI/FileSelector.h>

// Alpha engine
#include "GameLogic/EditorGameLogic.h"

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiSettings.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiInterface.h>


// Editor Core
#include "Editor.h"
#include "EditorData.h"
#include "Settings/IDESettings.h"
#include "Settings/ViewSettings.h"
#include "View/EditorView.h"
#include "Selection/EditorSelection.h"
#include "Plugin/EditorPlugin.h"
#include "Plugin/Scene/EPScene3D.h"
#include "Browser/ResourceBrowser.h"


// Editor UI
#include "../UI/Picker/ResourcePicker.h"
#include "../UI/Menu/MenuBarUI.h"
#include "../UI/Menu/MiniToolBarUI.h"
#include "../UI/Menu/ToolBarUI.h"
#include "../UI/Hierarchy/HierarchyWindow.h"
#include "../UI/Properties/GameAssetInspector.h"
#include "../UI/Hierarchy/AssetsHierarchyWindow.h"
#include "../UI/Tab/TabWindow.h"
#include "../UI/Attribute/AttributeContainer.h"
#include "../UI/Attribute/AttributeInspector.h"
#include "../UI/Modal/ModalWindow.h"
#include "../UI/Settings/ViewSettingsWindow.h"
#include "../UI/About/AboutTeamGDPWindow.h"
#include "../UI/Selector/GameAssetSelector.h"

#include "../UI/UIUtils.h"
#include "../UI/UIGlobals.h"

// Urho3d
#include <Urho3D/UI/FileSelector.h>

// Engine
#include "AlphaEngine/GameLogic/BaseGameLogic.h"

#include "../ImGuiUI/EditorImGuiInterface.h"
#include "../ImGuiUI/Menu/MenuBarImGuiUI.h"
#include "../ImGuiUI/ToolBar/MainToolBarImGuiUI.h"
#include "../ImGuiUI/Window/LeftFrameImGuiUI.h"
#include "../ImGuiUI/Window/RightFrameImGuiUI.h"


using namespace Urho3D;



// Register
void Editor::RegisterObject(Context* context)
{
    context->RegisterFactory<Editor>();

    return;
}

Editor::~Editor()
{
    DestroyAllDelegates();
}

Editor::Editor(Context* context) : Object(context)
    ,m_bIsVisible(false)
    ,m_bIsInitialized(false)
    ,m_pEditorPluginMain(NULL)
    ,m_pEditorPluginOver(NULL)
{

}

// Use Engine Std
void Editor::Initialize(void)
{
    m_pCache = g_pApp->GetConstantResCache();
    m_pUI = g_pApp->GetSubsystem<UI>();
    m_pGraphics = g_pApp->GetGraphics();
    m_pFileSystem = g_pApp->GetFileSystem();


    // Get default style - Resets style to new file
    XMLFile* xmlFile = m_pCache->GetResource<XMLFile>("UI/IDEStyle.xml"); // Changed default style to IDStyle.xml
    m_EditorDefaultStyleFilePath = "UI/IDEStyle.xml";
    m_EditorIconStyleFilePath = "UI/IDEIcons.xml";

    // Set Default stylefile
    if(xmlFile)
    {
        m_pUI->GetRoot()->SetDefaultStyle(xmlFile);
    }

}


void Editor::LoadPlugins()
{
    // Add 3D EPScene Plugin
    AddEditorPlugin(new EPScene3D(context_));
}

void Editor::AddEditorPlugin(EditorPlugin* plugin)
{
    if (plugin->HasMainScreen())
    {
        // push fist because tabwindow send tabchanged event on first add and that activates the first plugin.
        m_MainEditorPlugins.Push(plugin);
        if (m_pEditorView)
        {
            m_pEditorView->GetMiddleFrame()->AddTab(plugin->GetName(), plugin->GetMainScreen());
            URHO3D_LOGDEBUG("Plugin with name : " + plugin->GetName() + " and with mainscreen has beed added.");
        }
        else
        {
            URHO3D_LOGDEBUG("Failed to get editor view in AddPlugin()");
        }
    }
    URHO3D_LOGDEBUG("Plugin with name : " + plugin->GetName() + " and WITHOUT mainscreen has beed added.");
    m_pEditorData->AddEditorPlugin(plugin);
}

void Editor::RemoveEditorPlugin(EditorPlugin* plugin)
{
    if (plugin->HasMainScreen())
    {
        if (m_pEditorView)
        {
            m_pEditorView->GetMiddleFrame()->RemoveTab(plugin->GetName());
            m_MainEditorPlugins.Remove(plugin);
            URHO3D_LOGDEBUG("Plugin with name : " + plugin->GetName() + " and with mainscreen has beed deleted.");
        }
        else
        {
            URHO3D_LOGDEBUG("Failed to get editor view in DeletePlugin()");
        }
    }
    m_pEditorData->RemoveEditorPlugin(plugin);
    URHO3D_LOGDEBUG("Plugin with name : " + plugin->GetName() + " and WITHOUT mainscreen has beed deleted.");
}


bool Editor::Create(Scene* scene, UIElement* sceneUI)
{
    //////////////////////////////////////////////////////////////////////////
    /// Register all objects
    //////////////////////////////////////////////////////////////////////////

    Editor::RegisterObject(context_);
    ResourcePickerManager::RegisterObject(context_);
    EditorData::RegisterObject(context_);
    EditorView::RegisterObject(context_);
    EditorSelection::RegisterObject(context_);
    EPScene3DView::RegisterObject(context_);
    ViewSettings::RegisterObject(context_);


    // UI Relaated Objects
    MenuBarUI::RegisterObject(context_);
    ToolBarUI::RegisterObject(context_);
    MiniToolBarUI::RegisterObject(context_);

    // Window Related Objects
    ModalWindow::RegisterObject(context_);
    TabWindow::RegisterObject(context_);
    HierarchyWindow::RegisterObject(context_);

    GameAssetSelector::RegisterObject(context_);
    AboutTeamGDPWindow::RegisterObject(context_);

    // Get the game client context
    //  context_->RegisterSubsystem(new ImGuiInterface(context_));

    m_pGuiInterface = context_->GetSubsystem<ImGuiInterface>();


    // InitializeSettings
    m_pViewSettings = new ViewSettings(context_);

    VariantMap viewsettings_ = m_pViewSettings->ToVariantMap();

    //////////////////////////////////////////////////////////////////////////
    /// Create the scene
    //////////////////////////////////////////////////////////////////////////

    // Create editable scene and ui
    m_pSceneRootUI = sceneUI;

    // If no scene exist then create a scene
    if (scene == NULL)
    {
        m_pScene = new Scene(context_);
    }
    else
    {
        m_pScene = scene;
    }

    // Create Octree or Debug
    m_pScene->GetOrCreateComponent<Octree>();
    m_pScene->GetOrCreateComponent<DebugRenderer>();

    // Always pause the scene, and do updates manually
    m_pScene->SetUpdateEnabled(false);

    //////////////////////////////////////////////////////////////////////////
    /// Create the subsystems
    //////////////////////////////////////////////////////////////////////////

    // Create editor's resource picker manager system
    // ResourcePickerManager is needed for the Attribute Inspector, so don't forget to init it
    ResourcePickerManager* pResourcePickerManager = new ResourcePickerManager(context_);
    if (pResourcePickerManager)
    {
        context_->RegisterSubsystem(pResourcePickerManager);
        pResourcePickerManager->Init();
    }
    else
    {
        URHO3D_LOGERROR("Failed to create ResourcePickerManager. Check memory.");
        return false;
    }


    m_pEditorData = new EditorData(context_, this);
    if (m_pEditorData)
    {
        context_->RegisterSubsystem(m_pEditorData);
        m_pEditorData->Load();
    }
    else
    {
        URHO3D_LOGERROR("Failed to create EditorData. Check memory.");
        return false;
    }

    m_pRootUI = m_pEditorData->m_pRootUI;
    m_pEditorData->SetEditorScene(m_pScene);

    // Create editor's view system
    m_pEditorView = new EditorView(context_);
    if (m_pEditorView)
    {
        context_->RegisterSubsystem(m_pEditorView);
        if (!m_pEditorView->Initialize(this, m_pEditorData))
        {
            URHO3D_LOGERROR("Failed to initialize Editor view.");
            return false;
        }
    }
    else
    {
        URHO3D_LOGERROR("Failed to create EditorView. Check memory.");
        return false;
    }

    m_pEditorSelection = new EditorSelection(context_, this);
    if (m_pEditorSelection)
    {
        context_->RegisterSubsystem(m_pEditorSelection);
    }
    else
    {
        URHO3D_LOGERROR("Failed to create EditorSelection. Check memory.");
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    /// Create editor's ui
    //////////////////////////////////////////////////////////////////////////

    // Create base menu
    /*if (!CreateBaseMenu())
    {
        URHO3D_LOGERROR("Failed to create base menu.");
        return false;
    }

    // Create hierarchy window
    if (!CreateHierarchyWindow())
    {
        URHO3D_LOGERROR("Failed to create hierarchy window.");
        return false;
    }

    // Create attribute window
    if (!CreateAttributeWindow())
    {
        URHO3D_LOGERROR("Failed to create attribute window.");
        return false;
    }

    // Create resource browser
    if (!CreateResourceBrowser())
    {
        URHO3D_LOGERROR("Failed to create resource browser.");
        return false;
    }

    // Create game asset hierarchy
    if (!CreateGameAssetHierarchyWindow())
    {
        URHO3D_LOGERROR("Failed to create game asset hierarchy.");
        return false;
    }

    // Create game asset inspector
    if (!CreateGameAssetInspectorWindow())
    {
        URHO3D_LOGERROR("Failed to create game asset inspector.");
        return false;
    }

    // Create game asset inspector
    if (!CreateViewSettingsWindow())
    {
        URHO3D_LOGERROR("Failed to create view settings window.");

        return false;
    }
    */
    m_bIsVisible = true;
    m_bIsInitialized = true;

    InitializeAllDelegates();

    CreateMenuBar();
    CreateIcons();

    EditorCurrentToolBar=0;
    bShowTestWindow=false;

//    MainToolBarImGuiUI::SetToolBar(EditorToolBar, EditorToolBar);


    ResourceCache * ResCache = g_pApp ->GetConstantResCache();

    SharedPtr<File> file = ResCache->GetFile("Scenes/SceneLoadExample.xml");
    m_pScene->LoadXML(*file);

    LeftFrameImGuiUI::SetScene(m_pScene);

    return true;
}

void Editor::InitializeAllDelegates()
{
    if (m_pEditorView)
    {
        SubscribeToEvent(m_pEditorView->GetMenuBar(), E_MENUBAR_ACTION, URHO3D_HANDLER(Editor, HandleMenuBarActionDelegate));
    }

    if (m_pHierarchyWindow)
    {
        SubscribeToEvent(m_pHierarchyWindow->GetHierarchyList(), E_SELECTIONCHANGED, URHO3D_HANDLER(Editor, HandleHierarchyListSelectionChangeDelegate));
        SubscribeToEvent(m_pHierarchyWindow->GetHierarchyList(), E_ITEMDOUBLECLICKED, URHO3D_HANDLER(Editor, HandleHierarchyListDoubleClickDelegate));
    }

    if (m_pAssetsHierarchyWindow)
    {
        SubscribeToEvent(m_pAssetsHierarchyWindow->GetAssetsHierarchyList(), E_SELECTIONCHANGED, URHO3D_HANDLER(Editor, HandleHierarchyListSelectionChangeDelegate));

    }

    if (m_pEditorView)
    {
        SubscribeToEvent(m_pEditorView->GetMiddleFrame(), E_ACTIVETABCHANGED, URHO3D_HANDLER(Editor, HandleMainEditorTabChangedDelegate));
    }

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Editor, HandleUpdateDelegate));
}

void Editor::DestroyAllDelegates()
{
    UnsubscribeFromAllEvents();
}

bool Editor::CreateBaseMenu()
{
    MenuBarUI* pMenubar = m_pEditorView->GetMenuBar();

    if (!pMenubar)
    {
        return false;
    }

    // Set base maximum size to 1440
    /*pMenubar->SetMinWidth(1440);

    pMenubar->CreateMenu("File");
    pMenubar->CreateMenuItem("File", "Quit", A_QUITEDITOR_VAR);

    pMenubar->CreateMenuItem("File", "Export to AlphaEngine...", A_EXPORTTOALPHAENGINE_VAR);

    pMenubar->CreateMenuItem("File", "Set project directory...", A_SETPROJECTDIRECTORY_VAR);*/
    return true;
}


bool Editor::CreateHierarchyWindow()
{
    m_pHierarchyWindow = new HierarchyWindow(context_);

    if (!m_pHierarchyWindow)
    {
        URHO3D_LOGERROR("Failed to create HierarchyWindow. Check memory.");
        return false;
    }

    m_pHierarchyWindow->SetResizable(true);
    m_pHierarchyWindow->SetIconStyle(m_pEditorData->GetEditorIconStyle());
    m_pHierarchyWindow->SetTitle("Scene Hierarchy");
    m_pHierarchyWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
    m_pHierarchyWindow->SetStyleAuto();

    // Dont know why the auto style does not work ...
    m_pHierarchyWindow->SetTexture(m_pCache->GetResource<Texture2D>("Textures/UI.png"));
    m_pHierarchyWindow->SetImageRect(IntRect(112, 0, 128, 16));
    m_pHierarchyWindow->SetBorder(IntRect(2, 2, 2, 2));
    m_pHierarchyWindow->SetResizeBorder(IntRect(0, 0, 0, 0));
    m_pHierarchyWindow->SetLayoutSpacing(0);
    m_pHierarchyWindow->SetLayoutBorder(IntRect(0, 4, 0, 0));

    // Remove the title bar from the window
    m_pHierarchyWindow->SetTitleBarVisible(false);

    // Add Hierarchy inspector to the left side of the editor.
    m_pEditorView->GetLeftFrame()->AddTab("Hierarchy", m_pHierarchyWindow);

    // Connect the hierarchy with the editable scene.
    m_pHierarchyWindow->SetScene(m_pScene);

    // Connect the hierarchy with the editable ui.
    m_pHierarchyWindow->SetUIElement(m_pSceneRootUI);




    return true;
}


bool Editor::CreateAttributeWindow()
{
    //////////////////////////////////////////////////////////////////////////
    /// Create the attribute editor
    //////////////////////////////////////////////////////////////////////////

    m_pAttributeWindow = new AttributeInspector(context_);
    if (!m_pAttributeWindow)
    {
        URHO3D_LOGERROR("Failed to create Attribute inspector. Check memory.");
        return false;
    }

    Window* pAttribute = (Window*)m_pAttributeWindow->Create();
    if (!pAttribute)
    {
        URHO3D_LOGERROR("Failed to create Attribute window. Check m_pAttributeWindow Create() method.");
        return false;
    }

    pAttribute->SetResizable(false);
    pAttribute->SetMovable(false);

    // Remove the title bar from the window
    UIElement* titlebar = pAttribute->GetChild("TitleBar", true);

    if (titlebar)
    {
        titlebar->SetVisible(false);
    }

    // Add Attribute inspector to the right side of the editor.
    m_pEditorView->GetRightFrame()->AddTab("Inspector", pAttribute);

    return true;
}

bool Editor::CreateResourceBrowser()
{
    //////////////////////////////////////////////////////////////////////////
    /// create Resource Browser

    m_pResourceBrowser = new ResourceBrowser(context_);
    if (!m_pResourceBrowser)
    {
        URHO3D_LOGERROR("Failed to create Resource Browser. Check memory.");
        return false;
    }

    m_pResourceBrowser->CreateResourceBrowser();
    if (!m_pResourceBrowser->ShowResourceBrowserWindow())
    {
        URHO3D_LOGERROR("Failed to show Resource Browser. Check ShowResourceBrowserWindow() method.");
        return false;
    }

    return true;
}


bool Editor::CreateGameAssetHierarchyWindow()
{
    /// create the assetsHierarchy editor
    m_pAssetsHierarchyWindow = new AssetsHierarchyWindow(context_);
    m_pAssetsHierarchyWindow->SetResizable(true);
    m_pAssetsHierarchyWindow->SetIconStyle(m_pEditorData->GetEditorIconStyle());
    m_pAssetsHierarchyWindow->SetTitle("Assets Hierarchy");
    m_pAssetsHierarchyWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
    m_pAssetsHierarchyWindow->SetStyleAuto();

    // Dont know why the auto style does not work ...
    m_pAssetsHierarchyWindow->SetTexture(m_pCache->GetResource<Texture2D>("Textures/UI.png"));
    m_pAssetsHierarchyWindow->SetImageRect(IntRect(112, 0, 128, 16));

    m_pAssetsHierarchyWindow->SetBorder(IntRect(2, 2, 2, 2));
    m_pAssetsHierarchyWindow->SetResizeBorder(IntRect(0, 0, 0, 0));
    m_pAssetsHierarchyWindow->SetLayoutSpacing(0);
    m_pAssetsHierarchyWindow->SetLayoutBorder(IntRect(0, 4, 0, 0));

    /// remove the title bar from the window
    m_pAssetsHierarchyWindow->SetTitleBarVisible(false);

    /// add assetsHierarchy inspector to the left side of the editor.
    m_pEditorView->GetLeftFrame()->AddTab("Assets", m_pAssetsHierarchyWindow);

    /// connect the assetsHierarchy with the editable scene.
    m_pAssetsHierarchyWindow->SetScene(m_pScene);
    /// connect the assetsHierarchy with the editable ui.
    m_pAssetsHierarchyWindow->SetUIElement(m_pSceneRootUI);


    m_pAssetsHierarchyWindow->SetStyle("HierarchyWindow");          // Override Style to use the Hierarchy Window Style

    return true;
}

bool Editor::CreateGameAssetInspectorWindow()
{
    //////////////////////////////////////////////////////////////////////////
    // Create game asset attribute
    //////////////////////////////////////////////////////////////////////////
    m_pGameAssetInspectorWindow = new GameAssetInspector(context_);

    if (!m_pGameAssetInspectorWindow)
    {
        URHO3D_LOGERROR("Failed to create GameAsset inspector. Check memory.");
        return false;
    }

    m_pGameAssetInspectorWindow->SetResizable(true);
    m_pGameAssetInspectorWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
    m_pGameAssetInspectorWindow->SetIconStyle(m_pEditorData->GetEditorIconStyle());
    m_pGameAssetInspectorWindow->SetStyleAuto();

    // Dont know why the auto style does not work ...
    m_pGameAssetInspectorWindow->SetTexture(m_pCache->GetResource<Texture2D>("Textures/UI.png"));
    m_pGameAssetInspectorWindow->SetImageRect(IntRect(112, 0, 128, 16));
    m_pGameAssetInspectorWindow->SetBorder(IntRect(2, 2, 2, 2));
    m_pGameAssetInspectorWindow->SetResizeBorder(IntRect(0, 0, 0, 0));
    m_pGameAssetInspectorWindow->SetLayoutSpacing(0);
    m_pGameAssetInspectorWindow->SetLayoutBorder(IntRect(0, 4, 0, 0));

    m_pEditorView->GetRightFrame()->AddTab("Asset", m_pGameAssetInspectorWindow);

    /// connect the assetsHierarchy with the editable scene.
    m_pGameAssetInspectorWindow->SetScene(m_pScene);
    /// connect the assetsHierarchy with the editable ui.
    m_pGameAssetInspectorWindow->SetUIElement(m_pSceneRootUI);

    return true;
}


bool Editor::CreateViewSettingsWindow()
{
    //////////////////////////////////////////////////////////////////////////
    // Create game asset attribute
    //////////////////////////////////////////////////////////////////////////
    m_pViewSettingsWindow = new ViewSettingsWindow(context_);

    if (!m_pViewSettingsWindow)
    {
        URHO3D_LOGERROR("Failed to create View Settings inspector. Check memory.");
        return false;
    }

    m_pViewSettingsWindow->SetResizable(true);
    m_pViewSettingsWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
    m_pViewSettingsWindow->SetIconStyle(m_pEditorData->GetEditorIconStyle());
    m_pViewSettingsWindow->SetStyleAuto();


    m_pViewSettingsWindow->SetTexture(m_pCache->GetResource<Texture2D>("Textures/UI.png"));
    m_pViewSettingsWindow->SetImageRect(IntRect(112, 0, 128, 16));
    m_pViewSettingsWindow->SetBorder(IntRect(2, 2, 2, 2));
    m_pViewSettingsWindow->SetResizeBorder(IntRect(0, 0, 0, 0));
    m_pViewSettingsWindow->SetLayoutSpacing(0);
    m_pViewSettingsWindow->SetLayoutBorder(IntRect(0, 4, 0, 0));

    m_pEditorView->GetRightFrame()->AddTab("View", m_pViewSettingsWindow);

    /// connect the assetsHierarchy with the editable scene.
    m_pViewSettingsWindow->SetScene(m_pScene);
    /// connect the assetsHierarchy with the editable ui.
    m_pViewSettingsWindow->SetUIElement(m_pSceneRootUI);

    m_pViewSettingsWindow->UpdateSettings(m_pViewSettings);

    m_pViewSettingsWindow->SetEditor(this);




    return true;
}

// Create Settings menu
bool Editor::CreateSettingsMenu(void)
{
    /*  // Create Settings Menu
      MenuBarUI* pMenubar = m_pEditorView->GetMenuBar();

      if (!pMenubar)
      {
          return false;
      }

      // Create Menu
      pMenubar->CreateMenu("Settings");

      pMenubar->CreateMenuItem("Settings", "Preferences", A_SETTINGSPREFERENCES_VAR);
    */
    return true;
}

// Create About Menu
bool Editor::CreateAboutMenu(void)
{
    // Create about menu
    /*  MenuBarUI* pMenubar = m_pEditorView->GetMenuBar();

      if (!pMenubar)
      {
          return false;
      }

      // Create Menu
      pMenubar->CreateMenu("About");


      pMenubar->CreateMenuItem("About", "Garage Door Productions", A_ABOUTGDP_VAR);
      pMenubar->CreateMenuItem("About", "Developers", A_ABOUTGDPTEAM_VAR);

    */
    return true;
}

bool Editor::LoadScene(const String& fileName)
{
    if (!m_bIsInitialized)
    {
        return false;
    }

    if (fileName.Empty())
    {
        return false;
    }

    m_pUI->GetCursor()->SetShape(CS_BUSY);

    // Always load the scene from the filesystem, not from resource paths
    if (!m_pFileSystem->FileExists(fileName))
    {
        URHO3D_LOGERRORF("No such scene %s", fileName.CString());
        //MessageBox("No such scene.\n" + fileName);
        return false;
    }

    File file(context_);
    if (!file.Open(fileName, FILE_READ))
    {
        URHO3D_LOGERRORF("Could not open file %s", fileName.CString());
        //	MessageBox("Could not open file.\n" + fileName);
        return false;
    }

    // Reset stored script attributes.
    // 	scriptAttributes.Clear();
    //
    // 	// Add the scene's resource path in case it's necessary
    // 	String newScenePath = GetPath(fileName);
    // 	if (!rememberResourcePath || !sceneResourcePath.StartsWith(newScenePath, false))
    // 		SetResourcePath(newScenePath);

    m_pHierarchyWindow->SetSuppressSceneChanges(true);
    // 	sceneModified = false;
    // 	revertData = null;
    // 	StopSceneUpdate();

    String extension = GetExtension(fileName);
    bool loaded;
    if (extension != ".xml")
    {
        loaded = m_pScene->Load(file);
    }
    else
    {
        loaded = m_pScene->LoadXML(file);
    }

    // Release resources which are not used by the new scene
    // This creates an bug in the attribute inspector because the loaded xml files are released
    m_pCache->ReleaseAllResources(false);

    // Always pause the scene, and do updates manually
    m_pScene->SetUpdateEnabled(false);

    // 	UpdateWindowTitle();
    // 	DisableInspectorLock();
    m_pHierarchyWindow->UpdateHierarchyItem(m_pScene, true);

    // 	ClearEditActions();

    m_pHierarchyWindow->SetSuppressSceneChanges(false);


    m_pEditorSelection->ClearSelection();
    m_pAttributeWindow->GetEditNodes() = m_pEditorSelection->GetEditNodes();
    m_pAttributeWindow->GetEditComponents() = m_pEditorSelection->GetEditComponents();
    m_pAttributeWindow->GetEditUIElements() = m_pEditorSelection->GetEditUIElements();
    m_pAttributeWindow->Update();


    //
    // 	// global variable to mostly bypass adding mru upon importing tempscene
    // 	if (!skipMruScene)
    // 		UpdateSceneMru(fileName);
    //
    // 	skipMruScene = false;
    //
    // 	ResetCamera();
    // 	CreateGizmo();
    // 	CreateGrid();
    // 	SetActiveViewport(viewports[0]);
    //
    // 	// Store all ScriptInstance and LuaScriptInstance attributes
    // 	UpdateScriptInstances();

    return loaded;
}

Component* Editor::GetListComponentFromScene(UIElement* item)
{
    if (!m_bIsInitialized)
    {
        return NULL;
    }

    if (m_pScene.Null())
    {
        return NULL;
    }

    if (item == NULL)
    {
        return NULL;
    }

    if (item->GetVar(TYPE_VAR).GetInt() != ITEM_COMPONENT)
    {
        return NULL;
    }

    return m_pScene->GetComponent(item->GetVar(COMPONENT_ID_VAR).GetUInt());
}

Node* Editor::GetListNodeFromScene(UIElement* item)
{
    if (!m_bIsInitialized)
    {
        return NULL;
    }

    if (m_pScene.Null())
    {
        return NULL;
    }

    if (item == NULL)
    {
        return NULL;
    }

    if (item->GetVar(TYPE_VAR).GetInt() != ITEM_NODE)
    {
        return NULL;
    }

    return m_pScene->GetNode(item->GetVar(NODE_ID_VAR).GetUInt());
}

UIElement* Editor::GetListUIElementFromUIScene(UIElement*  item)
{
    if (!m_bIsInitialized)
    {
        return NULL;
    }

    if (m_pScene.Null())
    {
        return NULL;
    }

    if (item == NULL)
    {
        return NULL;
    }

    // Get the text item's ID and use it to retrieve the actual UIElement the text item is associated to
    return GetUIElementByID(UIUtils::GetUIElementID(item));
}

UIElement* Editor::GetUIElementByID(const Variant& id)
{
    return id == UI_ELEMENT_BASE_ID ? NULL : m_pSceneRootUI->GetChild(UI_ELEMENT_ID_VAR, id, true);
}

void Editor::CreateFileSelector(const String& title, const String& ok, const String& cancel, const String& initialPath, Vector<String>& filters, unsigned int initialFilter, bool directoryMode)
{
    // Within the editor UI, the file selector is a kind of a "singleton". When the previous one is overwritten, also
    // the events subscribed from it are disconnected, so new ones are safe to subscribe.
    m_pUIFileSelector = new FileSelector(context_);
    m_pUIFileSelector->SetDefaultStyle(m_pCache->GetResource<XMLFile>("UI/IDEStyle.xml"));
    m_pUIFileSelector->SetTitle(title);
    m_pUIFileSelector->SetPath(initialPath);
    m_pUIFileSelector->SetButtonTexts(ok, cancel);
    m_pUIFileSelector->SetFilters(filters, initialFilter);
    m_pUIFileSelector->SetDirectoryMode(directoryMode);

    IntVector2 size = m_pUIFileSelector->GetWindow()->GetSize();
    m_pUIFileSelector->GetWindow()->SetPosition((m_pGraphics->GetWidth() - size.x_) / 2, (m_pGraphics->GetHeight() - size.y_) / 2);
}

void Editor::CloseFileSelector(unsigned int& filterIndex, String& path)
{
    // Save filter & path for next time
    filterIndex = m_pUIFileSelector->GetFilterIndex();
    path = m_pUIFileSelector->GetPath();

    m_pUIFileSelector = NULL;
}

void Editor::HandleUpdateDelegate(StringHash eventType, VariantMap& eventData)
{
    if(g_pApp->GetImGuiInterface())
    {
        MenuBarImGuiUI::ShowMenuBar(EditorMenuBar);
        //MainToolBarImGuiUI::ShowToolBar(bEditorToolBar,EditorToolBar);
        MainToolBarImGuiUI::ShowToolBar(EditorCurrentToolBar,EditorToolBar);
        LeftFrameImGuiUI::ShowLeftFrame();
        RightFrameImGuiUI::ShowRightFrame();

        if(bShowTestWindow==true)
        {
            ImGui::ShowTestWindow();
        }

    }

    using namespace Update;
    float timestep = eventData[P_TIMESTEP].GetFloat();

    if (m_pEditorPluginMain)
    {
        m_pEditorPluginMain->Update(timestep);
    }

    if (m_pEditorPluginOver)
    {
        m_pEditorPluginOver->Update(timestep);
    }

    /*    if (m_pResourceBrowser->IsVisible())
        {
            m_pResourceBrowser->Update();
        }*/

    // Handle editor keypress check
    HandleKeyDownDelegate(eventType,eventData);
}

void Editor::HandleMenuBarActionDelegate(StringHash eventType, VariantMap& eventData)
{
    using namespace MenuBarAction;

    StringHash action = eventData[P_ACTION].GetStringHash();
    if (action == A_QUITEDITOR_VAR)
    {
        // Create a event and send it for login can listen to it
        VariantMap  editorInstanceEvent;
        editorInstanceEvent[EditorInstanceEvent::P_ACTION] = -1;

        // Send the event
        SendEvent(E_EDITORINSTANCE, editorInstanceEvent);
    }
    else if (action == A_EXPORTTOALPHAENGINE_VAR)
    {
        // Get selected node
        if (!ExportToAlphaEngine())
        {
            URHO3D_LOGERROR("Failed to export GameAsset to AlphaEngine");
        }
    }
    else if (action == A_SETPROJECTDIRECTORY_VAR)
    {
        CreateFileSelector("Set project directory", "Set", "Cancel", m_pEditorData->GetProgramDirPath(), m_pEditorData->GetUIAllFilters(), m_pEditorData->GetUIImportFilter(), true);
        SubscribeToEvent(GetUIFileSelector(), E_FILESELECTED, URHO3D_HANDLER(Editor, HandleSetProjectDirectoryDelegate));
    }
    else if (action == A_SHOWHIERARCHY_VAR)
    {

    }
    else if (action == A_SHOWATTRIBUTE_VAR)
    {

    }
}

void Editor::HandleSetProjectDirectoryDelegate(StringHash eventType, VariantMap& eventData)
{
    CloseFileSelector(m_pEditorData->GetUIImportFilter(), m_pEditorData->GetProgramDirPath());
    AddResourcePath(UIUtils::ExtractFileName(eventData));
}

void Editor::HandleMainEditorTabChangedDelegate(StringHash eventType, VariantMap& eventData)
{
    using namespace ActiveTabChanged;

    unsigned index = eventData[P_TABINDEX].GetUInt();

    if (index >= m_MainEditorPlugins.Size())
    {
        URHO3D_LOGERROR("Failed to handle main editor tab change. Index is out of bound.");
        return; // error ...
    }

    EditorPlugin* pPlugin = m_MainEditorPlugins[index];
    if (!pPlugin)
    {
        URHO3D_LOGERROR("Failed to handle main editor tab change. Can't find plugin by id.");
        return; // error
    }


    if (m_pEditorPluginMain == pPlugin)
    {
        URHO3D_LOGDEBUG("Failed to handle main editor tab change. MainEditor plugin is equal to changed plugin.");
        return; // do nothing
    }

    if (m_pEditorPluginMain)
    {
        m_pEditorPluginMain->SetVisible(false);
    }

    m_pEditorPluginMain = pPlugin;
    m_pEditorPluginMain->SetVisible(true);

    // editorPluginMain_->selectedNotify(); edit
}

void Editor::HandleHierarchyListSelectionChangeDelegate(StringHash eventType, VariantMap& eventData)
{
    m_pEditorSelection->OnHierarchyListSelectionChange(m_pHierarchyWindow->GetHierarchyList()->GetItems(), m_pHierarchyWindow->GetHierarchyList()->GetSelections());

    // Dont copy
    m_pAttributeWindow->GetEditNodes() = m_pEditorSelection->GetEditNodes();
    m_pAttributeWindow->GetEditComponents() = m_pEditorSelection->GetEditComponents();
    m_pAttributeWindow->GetEditUIElements() = m_pEditorSelection->GetEditUIElements();
    m_pAttributeWindow->Update();

    // 	OnSelectionChange();
    //
    // 	// 		PositionGizmo();
    // 	UpdateAttributeInspector();
    // 	// 		UpdateCameraPreview();
}

void Editor::HandleHierarchyListDoubleClickDelegate(StringHash eventType, VariantMap& eventData)
{
    using namespace ItemDoubleClicked;
    UIElement* item = dynamic_cast<UIElement*>(eventData[P_ITEM].GetPtr());


}

void Editor::AddResourcePath(String newPath, bool usePreferredDir /*= true*/)
{
    if (newPath.Empty())
    {
        return;
    }

    if (!IsAbsolutePath(newPath))
    {
        newPath = m_pFileSystem->GetCurrentDir() + newPath;
    }

    if (usePreferredDir)
    {
        newPath = AddTrailingSlash(m_pCache->GetPreferredResourceDir(newPath));
    }
    else
    {
        newPath = AddTrailingSlash(newPath);
    }

    // If additive (path of a loaded prefab) check that the new path isn't already part of an old path
    Vector<String> resourceDirs = m_pCache->GetResourceDirs();

    for (unsigned int i = 0; i < resourceDirs.Size(); ++i)
    {
        if (newPath.StartsWith(resourceDirs[i], false))
        {
            return;
        }
    }

    // Add resource path as first priority so that it takes precedence over the default data paths
    m_pCache->AddResourceDir(newPath, 0);

    m_pResourceBrowser->RebuildResourceDatabase();
    //RebuildResourceDatabase();
}

bool Editor::ExportToAlphaEngine()
{
    CreateFileSelector("Select directory", "Save", "Cancel", m_pEditorData->GetProgramDirPath(), m_pEditorData->GetUIAllFilters(), m_pEditorData->GetUIImportFilter(), true);
    SubscribeToEvent(GetUIFileSelector(), E_FILESELECTED, URHO3D_HANDLER(Editor, HandleExportGameAssetDelegate));
    return true;
}

// Handle Key Presses
void Editor::HandleKeyDownDelegate(StringHash eventType, VariantMap& eventData)
{
    // Get input subsystem
    Input * input_ = g_pApp->GetSubsystem<Input>();

    if (input_->GetKeyPress(KEY_TAB)==true)
    {
        if(EditorCurrentToolBar==0)
        {
            EditorCurrentToolBar=1;
        }
        else
        {
            EditorCurrentToolBar=0;
        }
    }


    if (input_->GetKeyPress(KEY_F10)==true)
    {

        if(EditorCurrentToolBar==false)
        {
            bShowTestWindow=true;
        }
        else
        {
            bShowTestWindow=false;
        }
    }


    // Check for delete key was pressed
    if (input_->GetKeyPress(KEY_DELETE)==true)
    {

        // Edit nodes
        //Vector<WeakPtr<Node>>&	editNodes = m_pEditorSelection->GetEditNodes();
        Vector<WeakPtr<Node>>&  selectedNodes = m_pEditorSelection->GetSelectedNodes();

        // Check if there are selected items
        if(selectedNodes.Size()>0)
        {

            // Get the initial node ID
            unsigned id=selectedNodes.At(0)->GetID();


            // Check if the node is not blank or the screen
            if(id!=1&&id!=0)
            {

                // if the plugin is set
                if(m_pEditorPluginMain)
                {

                    // Stop any plugin from update to execute delete
                    m_pEditorPluginMain->StopSceneUpdate();

                    // Should stop some scene updating
                    if(m_pScene)
                    {
                        // Get first valid node to delete
                        Node * deleteNode = selectedNodes.At(0);

                        // Clear selection if not it will crash
                        m_pEditorSelection->ClearSelection();

                        // Remove the node
                        deleteNode->RemoveAllChildren();
                        deleteNode->Remove();

                    }

                    // Start scene updating
                    m_pEditorPluginMain->StopSceneUpdate();
                }
            }
        }
    }
    return;
}

void Editor::HandleExportGameAssetDelegate(StringHash eventType, VariantMap& eventData)
{
    CloseFileSelector(m_pEditorData->GetUIImportFilter(), m_pEditorData->GetProgramDirPath());
    m_ExportDirectory = UIUtils::ExtractFileName(eventData);

    Vector<WeakPtr<Node>>& nodes = m_pEditorSelection->GetSelectedNodes();
    Vector<WeakPtr<Node>>::ConstIterator it = nodes.Begin();
    for (; it != nodes.End(); it++)
    {
        pugi::xml_document document;
        String nodeName = (*it)->GetName();
        GameNodeId nodeId = (*it)->GetID();
        g_pApp->GetGameLogic()->GetGameAssetXml(document, nodeId, m_pScene);
        document.save_file((m_ExportDirectory + nodeName + ".xml").CString());
    }
}

// Update settings
void Editor::Refresh3DViewSettings(void)
{
    // If the viewscreen plugin is loaded
    if (m_pEditorPluginMain)
    {
        m_pEditorPluginMain->Update3DViewSettings();
    }

}

void Editor::CreateMenuBar()
{
    // create struct
    // create fist submenu
    SubMenu SubMenuAlpha;
    SubMenuAlpha.Options.Push((MenuItem)
    {"AlphaEditor",0,true
    });
    SubMenuAlpha.Options.Push((MenuItem)
    {"Export to AlphaEngine###ExportAlphaEngine",0,true
    });
    SubMenuAlpha.Options.Push((MenuItem)
    {"Exit###ExitAlphaEditor",0,true
    });


    SubMenu SubMenuFile;
    SubMenuFile.Options.Push((MenuItem)
    {"File",0,true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"New scene###File_NewScene", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Open scene...###File_OpenScene", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Save scene###File_SaveScene", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Save scene as...###File_SaveSceneAs", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Load node as replicated###File_LoadNodeReplicated", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Load node as local###File_LoadNodeLocal", 0, true
    });
    SubMenuFile.Options.Push((MenuItem)
    {"Save node as###File_SaveNodeAs", 0, true
    });

    SubMenu SubMenuCreate;
    SubMenuCreate.Options.Push((MenuItem)
    {"Create",0,true
    });
    SubMenuCreate.Options.Push((MenuItem)
    {"Replicated node###Create_ReplicatedNode", 0, true
    });
    SubMenuCreate.Options.Push((MenuItem)
    {"Local node###Create_LocalNode", 0, true
    });
    SubMenuCreate.Options.Push((MenuItem)
    {"Game Asset###Create_GameAsset", 0, true
    });

    SubMenu SubMenuEdit;
    SubMenuEdit.Options.Push((MenuItem)
    {"Edit",0,true
    });
    SubMenuEdit.Options.Push((MenuItem)
    {"Copy###Edit_Copy", 0, true
    });
    SubMenuEdit.Options.Push((MenuItem)
    {"Paste###Edit_Paste", 0, true
    });

    SubMenu SubMenuTools;
    SubMenuTools.Options.Push((MenuItem)
    {"Tools",0,true
    });
    SubMenuTools.Options.Push((MenuItem)
    {"Materials###Tools_MaterialEditor",0,true
    });
    SubMenuTools.Options.Push((MenuItem)
    {"Particle###Tools_ParticleEditor",0,true
    });

    SubMenuTools.Options.Push((MenuItem)
    {"Sound###Tools_SoundEditor",0,true
    });

    SubMenu SubMenuPreferences;
    SubMenuPreferences.Options.Push((MenuItem)
    {"Preferences",0,true
    });
    SubMenuPreferences.Options.Push((MenuItem)
    {"Configuration###Preferences_Configuration",0,true
    });

    SubMenu SubMenuAbout;
    SubMenuAbout.Options.Push((MenuItem)
    {"About",0,true
    });
    SubMenuAbout.Options.Push((MenuItem)
    {"Garage Door Productions###About_GarageDoorProductions", 0, true
    });
    SubMenuAbout.Options.Push((MenuItem)
    {"Alpha Dev Team###About_DevTeam", 0, true
    });


    EditorMenuBar.Push(SubMenuAlpha);
    EditorMenuBar.Push(SubMenuFile);
    EditorMenuBar.Push(SubMenuCreate);
    EditorMenuBar.Push(SubMenuEdit);
    EditorMenuBar.Push(SubMenuTools);
    EditorMenuBar.Push(SubMenuPreferences);
    EditorMenuBar.Push(SubMenuAbout);


    return;
}


void Editor::CreateIcons()
{


    // create fist submenu
    ToolBarRow ToolBar0;
    ToolBar0.Options.Push((ToolBarIcon)
    {"1###ToolBar_ToggleBar", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"A###ToolBar_RunUpdatePlay", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"U###ToolBar_RunUpdatePause", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"W###ToolBar_RevertOnPause", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"A###ToolBar_EditMove", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"B###ToolBar_EditRotate", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"C###ToolBar_EditScale", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"D###ToolBar_EditSelect", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"E###ToolBar_AxisWorld", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"F###ToolBar_AxisLocal", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"G###ToolBar_MoveSnap", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"H###ToolBar_RotateSnap", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"I###ToolBar_ScaleSnap", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"O###ToolBar_SnapScaleHalf", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"P###ToolBar_SnapScaleQuarter", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"J###ToolBar_PickGeometries", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"K###ToolBar_PickLight", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"L###ToolBar_PickZone", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"M###ToolBar_PickRigidBodies", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"N###ToolBar_PickUIElements", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"Q###ToolBar_FillPoint", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"R###ToolBar_FillWireframe", false
    });
    ToolBar0.Options.Push((ToolBarIcon)
    {"S###ToolBar_FillSolid", false
    });


    // create fist submenu
    ToolBarRow ToolBar1;
    ToolBar1.Options.Push((ToolBarIcon)
    {"1###ToolBar_ToggleBar", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"a###ToolBar_ReplicatedNode", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"b###ToolBar_LocalNode", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"$###ToolBar_GameAsset", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"c###ToolBar_Light", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"d###ToolBar_Camera", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"e###ToolBar_Zone", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"f###ToolBar_StaticModel", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"g###ToolBar_AnimatedModel", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"h###ToolBar_BillBoardSet", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"i###ToolBar_ParticleEmitter", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"j###ToolBar_Skybox", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"k###ToolBar_Terrain", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"l###ToolBar_Text3D", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"m###ToolBar_SoundListener", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"n###ToolBar_SoundSource3D", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"o###ToolBar_SoundSource", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"p###ToolBar_RigidBody", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"q###ToolBar_CollisionShape", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"r###ToolBar_Constraint", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"s###ToolBar_AnimationController", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"t###ToolBar_ScriptInstance", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"u###ToolBar_Navigable", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"v###ToolBar_NavigationMesh", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {" ", true
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"w###ToolBar_OffMeshConnection", false
    });
    ToolBar1.Options.Push((ToolBarIcon)
    {"x###ToolBar_NetworkPriority", false
    });


    EditorToolBar.Push(ToolBar0);
    EditorToolBar.Push(ToolBar1);

    return;
}
