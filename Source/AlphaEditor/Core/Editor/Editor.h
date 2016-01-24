#pragma once


#include "AlphaEditorStd.h"


#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiInterface.h>
#include <AlphaEngine/Interfaces/ImGui/ImGuiInterface.h>

/// Menu selected.
URHO3D_EVENT(E_EDITORINSTANCE, EditorInstanceEvent)
{
    URHO3D_PARAM(P_ACTION, Action);              // stringhash
}

// Forward reference
class HierarchyWindow;
class AssetsHierarchyWindow;
class GameAssetInspector;
class AttributeInspector;
class EditorSelection;
class EditorData;
class EditorView;
class ViewSettings;
class ViewSettingsWindow;

class ToolBarUI;
class MiniToolBarUI;
class MenuBarUI;
class TabWindow;
class ResourceBrowser;




#include "Plugin/EditorPlugin.h"

#include <Urho3D/UI/FileSelector.h>



// menu items that would bar ImGui Friendly
struct MenuItem
{
    String MenuLabel;
    char HotKey;
    bool Enabled;
};

// Replacement for menu
struct SubMenu
{
    Vector<MenuItem> Options;
};

struct ToolBarIcon
{
    char Letter;
    bool Separator;
};

struct ToolBarRow
{
    Vector<ToolBarIcon> Options;
};



class Editor : public Object
{
    URHO3D_OBJECT(Editor, Object);
public:
    // Constructor
    Editor(Context* context);
    // Destructor
    virtual ~Editor();
    // Register for Urho3D::Factory
    static void RegisterObject(Context* context);

public:
    // First, initialize editor
    virtual void Initialize(void);
    // Second, create editor's scene and UI
    bool Create(Scene* scene, UIElement* sceneUI);
    // Third, load startup plugins
    void LoadPlugins();
    // Load scene
    bool LoadScene(const String& fileName);

    // Create base menu
    bool CreateBaseMenu();
    // Create hierarchy window
    bool CreateHierarchyWindow();
    // Create attribute window
    bool CreateAttributeWindow();
    // Create resource browser
    bool CreateResourceBrowser();
    // Create game asset hierarchy window
    bool CreateGameAssetHierarchyWindow();
    // Create game asset inspector window
    bool CreateGameAssetInspectorWindow();
    // Create view settigns window
    bool CreateViewSettingsWindow();
    // Create about menu
    bool CreateAboutMenu(void);
    // Create settings menu
    bool CreateSettingsMenu(void);

public:
    // Getters/Setters
    // Get editor data
    EditorData* GetEditorData()
    {
        return m_pEditorData;
    }
    // Get editor view
    EditorView* GetEditorView()
    {
        return m_pEditorView;
    }
    // Get editor default style file path
    const String& GetEditorDefaultStylePath()
    {
        return m_EditorDefaultStyleFilePath;
    }
    // Get editor icon style file path
    const String& GetEditorIconStylePath()
    {
        return m_EditorIconStyleFilePath;
    }

    ViewSettings* GetViewSettings()
    {
        return m_pViewSettings;
    }

    // Get scene
    Scene* GetScene()
    {
        return m_pScene;
    }
    // Set scene
    void SetScene(Scene* scene)
    {
        m_pScene = scene;
    }
    // Get UI scene
    UIElement* GetSceneUI()
    {
        return m_pSceneRootUI;
    }
    // Set UI scene
    void SetSceneUI(UIElement* sceneUI)
    {
        m_pSceneRootUI = sceneUI;
    }

    // Get component from scene by UI item
    Component* GetListComponentFromScene(UIElement* item);
    // Get node from scene by UI item
    Node* GetListNodeFromScene(UIElement* item);
    // Get UI element from UI scene by UI item
    UIElement* GetListUIElementFromUIScene(UIElement* item);
    // Get UI element from UI scene by id
    UIElement* GetUIElementByID(const Variant& id);

    // Get hierarchy window
    HierarchyWindow* GetHierarchyWindow()
    {
        return m_pHierarchyWindow;
    }
    // Get attribute window
    AttributeInspector* GetAttributeWindow()
    {
        return m_pAttributeWindow;
    }
    // Get editor's selection
    EditorSelection* GetEditorSelection()
    {
        return m_pEditorSelection;
    }

    // Adds the plugin to the editor data, if plugin has the main screen then add it to the middle frame tabs.
    void AddEditorPlugin(EditorPlugin* plugin);
    // Remove the plugin
    void RemoveEditorPlugin(EditorPlugin* plugin);

    // UI Stuff
    // Create/add a Menu Item to the Menu Bar
    bool AddToMenuBar(const String& menuTitle, const String& itemTitle, const StringHash& action, int accelKey, int accelQual, bool addToQuickMenu, String quickMenuText);
    // Create/add a Toggle to the Tool Bar
    bool AddToToolBar(const String& groupname, const String& title);
    // Create/add a Spacer to the Tool Bar
    void AddSpacerToToolBar(int width);

    // Get file selector
    Urho3D::FileSelector* GetUIFileSelector()
    {
        return m_pUIFileSelector;
    }
    // Create file selector
    void CreateFileSelector(const String& title, const String& ok, const String& cancel, const String& initialPath, Vector<String>& filters, unsigned int initialFilter, bool directoryMode = false);
    // Close file selector (get filterIndex anad path)
    void CloseFileSelector(unsigned int& filterIndex, String& path);
    // Close file selector
    void CloseFileSelector()
    {
        m_pUIFileSelector.Reset();
    }

    // Refresh editor
    void Refresh3DViewSettings(void);

    //
    void CreateMenuBar();
    void CreateIcons();

protected:
    // Export to AlphaEngine selected GameAssets
    bool ExportToAlphaEngine();

    // Add Resource Path
    void AddResourcePath(String newPath, bool usePreferredDir = true);

    // Delegates
    // Initialize all delegates
    void InitializeAllDelegates();
    // Destroy all delegates
    void DestroyAllDelegates();

    // Handle Updates events
    void HandleUpdateDelegate(StringHash eventType, VariantMap& eventData);
    // Handle Menu Bar events
    void HandleMenuBarActionDelegate(StringHash eventType, VariantMap& eventData);
    // Handle Tab change events
    void HandleMainEditorTabChangedDelegate(StringHash eventType, VariantMap& eventData);
    // Handle Hierarchy selection events
    void HandleHierarchyListSelectionChangeDelegate(StringHash eventType, VariantMap& eventData);
    // Handle Hierarchy double click events
    void HandleHierarchyListDoubleClickDelegate(StringHash eventType, VariantMap& eventData);
    // Handle set project directory
    void HandleSetProjectDirectoryDelegate(StringHash eventType, VariantMap& eventData);
    // Handle Key Down
    void HandleKeyDownDelegate(StringHash eventType, VariantMap& eventData);
    // Handle game asset export
    void HandleExportGameAssetDelegate(StringHash eventType, VariantMap& eventData);

    //
protected:
    // Cached pointer from Engine
    ResourceCache* m_pCache;
    // Cached pointer from Engine
    UI* m_pUI;
    // Cached pointer from Engine
    Graphics* m_pGraphics;
    // Cached pointer from Engine
    FileSystem* m_pFileSystem;

    // Is the editor visible, used for the in game editor
    bool m_bIsVisible;
    // IS the editor initialized
    bool m_bIsInitialized;

    // Currently edited scene
    SharedPtr<Scene> m_pScene;
    // Currently edited scene through UI.
    SharedPtr<UIElement> m_pSceneRootUI;

    // UI Root element
    SharedPtr<UIElement> m_pRootUI;
    // Editor view
    SharedPtr<EditorView> m_pEditorView;
    // File selector
    SharedPtr<Urho3D::FileSelector> m_pUIFileSelector;

    // Editor's selection (Split editor functionality in different classes)
    SharedPtr<EditorSelection> m_pEditorSelection;
    // Editor's data (Split editor functionality in different classes)
    SharedPtr<EditorData> m_pEditorData;

    // Hierarchy window (Default IDE Editors)
    SharedPtr<HierarchyWindow> m_pHierarchyWindow;
    // Attribute window (Default IDE Editors)
    SharedPtr<AttributeInspector> m_pAttributeWindow;
    //WeakPtr<AttributeInspector> m_pAttributeWindow;
    // Resource browser (Default IDE Editors)
    SharedPtr<ResourceBrowser> m_pResourceBrowser;
    // Game asset inspector window (Default IDE Editors)
    SharedPtr<GameAssetInspector> m_pGameAssetInspectorWindow;
    // Game asset hierarchy window (Default IDE Editors)
    SharedPtr<AssetsHierarchyWindow> m_pAssetsHierarchyWindow;

    // View Settings
    SharedPtr<ViewSettingsWindow> m_pViewSettingsWindow;

    // All editor plugins (Editor's plugin handling)
    Vector<EditorPlugin*> m_MainEditorPlugins;
    // Main editor's plugin
    EditorPlugin* m_pEditorPluginMain;
    // Over editor's plugin
    EditorPlugin* m_pEditorPluginOver;

    // Path to default editor style file
    String m_EditorDefaultStyleFilePath;
    // Path to editor's icon style file
    String m_EditorIconStyleFilePath;

    // View Settings
    SharedPtr<ViewSettings>m_pViewSettings;

    // Export directory
    String m_ExportDirectory;

    SharedPtr<ImGuiInterface> m_pGuiInterface;

    // Imgui related
    Vector<SubMenu> EditorMenuBar;
    unsigned int bEditorToolBar;
    Vector<ToolBarRow> EditorToolBar;

    bool * m_pShowToolBar;
};
