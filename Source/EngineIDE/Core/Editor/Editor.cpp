#include "EngineIDEStd.h"

// Urho3d
#include <Urho3D/UI/FileSelector.h>

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

#include "../UI/UIUtils.h"
#include "../UI/UIGlobals.h"

// Urho3d
#include <Urho3D/UI/FileSelector.h>

// Engine
#include "EngineStd/GameLogic/BaseGameLogic.h"


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


    // InitializeSettings
    m_pViewSettings = new ViewSettings(context_);

    VariantMap viewsettings_ = m_pViewSettings->ToVariantMap();

    //URHO3D_LOGINFO(String(viewsettings_["cameraBaseSpeed_"].GetFloat()));

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
	if (!CreateBaseMenu())
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

    m_bIsVisible = true;
	m_bIsInitialized = true;

	InitializeAllDelegates();

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
    pMenubar->SetMinWidth(1440);

	pMenubar->CreateMenu("File");
	pMenubar->CreateMenuItem("File", "Quit", A_QUITEDITOR_VAR);

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

	//SubscribeToEvent(assetsHierarchyWindow_->GetAssetsHierarchyList(), E_SELECTIONCHANGED, URHO3D_HANDLER(Editor, HandleAssetsHierarchyListSelectionChange));
	//SubscribeToEvent(assetsHierarchyWindow_->GetAssetsHierarchyList(), E_ITEMDOUBLECLICKED, URHO3D_HANDLER(Editor, HandleAssetsHierarchyListDoubleClick));

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
	//gameAssetInspectorWindow_->SetIconStyle(editorData_->iconStyle_);
//	m_pGameAssetInspectorWindow->SetTitle("Asset Inspector");
	m_pGameAssetInspectorWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
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
	//ViewSettingsWindow_->SetIconStyle(editorData_->iconStyle_);
	//ViewSettingsWindow_->SetTitle("Scene Hierarchy");
	m_pViewSettingsWindow->SetDefaultStyle(m_pEditorData->GetEditorDefaultStyle());
	m_pViewSettingsWindow->SetStyleAuto();

	// Dont know why the auto style does not work ...
	m_pViewSettingsWindow->SetTexture(m_pCache->GetResource<Texture2D>("Textures/UI.png"));
	m_pViewSettingsWindow->SetImageRect(IntRect(112, 0, 128, 16));
	m_pViewSettingsWindow->SetBorder(IntRect(2, 2, 2, 2));
	m_pViewSettingsWindow->SetResizeBorder(IntRect(0, 0, 0, 0));
	m_pViewSettingsWindow->SetLayoutSpacing(0);
	m_pViewSettingsWindow->SetLayoutBorder(IntRect(0, 4, 0, 0));

	//m_pViewSettingsWindow->SetStyleAuto();

	m_pEditorView->GetRightFrame()->AddTab("View", m_pViewSettingsWindow);


    /// connect the assetsHierarchy with the editable scene.
	m_pViewSettingsWindow->SetScene(m_pScene);
	/// connect the assetsHierarchy with the editable ui.
	m_pViewSettingsWindow->SetUIElement(m_pSceneRootUI);

    m_pViewSettingsWindow->UpdateSettings(m_pViewSettings);

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

void Editor::CreateFileSelector(const String& title, const String& ok, const String& cancel, const String& initialPath, Vector<String>& filters, unsigned int initialFilter)
{
    // Within the editor UI, the file selector is a kind of a "singleton". When the previous one is overwritten, also
    // the events subscribed from it are disconnected, so new ones are safe to subscribe.
    m_pUIFileSelector = new FileSelector(context_);
	m_pUIFileSelector->SetDefaultStyle(m_pCache->GetResource<XMLFile>("UI/IDEStyle.xml"));
	m_pUIFileSelector->SetTitle(title);
	m_pUIFileSelector->SetPath(initialPath);
	m_pUIFileSelector->SetButtonTexts(ok, cancel);
	m_pUIFileSelector->SetFilters(filters, initialFilter);

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

	if (m_pResourceBrowser->IsVisible())
	{
		m_pResourceBrowser->Update();
	}
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
    else if (action == A_SHOWHIERARCHY_VAR)
    {

    }
    else if (action == A_SHOWATTRIBUTE_VAR)
    {

    }
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
    //RebuildResourceDatabase();
}
