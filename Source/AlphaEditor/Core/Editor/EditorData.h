#pragma once

class EditorPlugin;

class EditorData : public Object
{
    friend class Editor;
    URHO3D_OBJECT(EditorData,Object);

public:
	// Constructor
	EditorData(Context* context, Editor* editor = NULL);
	// Destructor
	virtual ~EditorData();
	// Register for Urho3D::Factory
	static void RegisterObject(Context* context);

	// Load data for editor
	bool Load(Editor* editor);
	// Load data for editor
	bool Load();

    // Add editor plugin
	void AddEditorPlugin(EditorPlugin* plugin) { m_EditorPlugins.Push(SharedPtr<EditorPlugin>(plugin)); }
    // Remove editor plugin
	void RemoveEditorPlugin(EditorPlugin* plugin) { m_EditorPlugins.Remove(SharedPtr<EditorPlugin>(plugin)); }

	//Getters/Setters
	// Get editor
	Editor*	GetEditor() { return m_pEditor; }

	// Get the editable scene.
	Scene* GetEditorScene() { return m_pScene; }
	// Set the editable scene.
	void SetEditorScene(Scene* scene) { m_pScene = scene; }

	// Get editor's UI root
	UIElement* GetEditorRootUI() { return m_pRootUI; }

	// Get editor default style XML file
	XMLFile* GetEditorDefaultStyle() { return m_pEditorDefaultStyle; }
	// Get editor icon style XML file
	XMLFile* GetEditorIconStyle() { return m_pEditorIconStyle; }

	// Return an editor plugin that can handle this object and has the main screen (middle frame).
	EditorPlugin* GetEditorPlugin(Object* object);
	// Return an editor plugin that can handle this object and is not in the middle frame tabs.
	EditorPlugin* GetSubeditorPlugin(Object* object);
	// Return an editor plugin by name.
	EditorPlugin* GetEditorPlugin(const String& name);


	// Get editor's global variables by name
	const Variant& GetGlobalVarNames(StringHash& name) { return m_GlobalVarNames[name]; }
	// Set editor's global variables by name
	void SetGlobalVarNames(const String& name) { m_GlobalVarNames[name] = name; }


	// Get scene filters
	Vector<String>& GetUISceneFilters() { return m_UISceneFilters; }
	// Get element filters
	Vector<String>& GetUIElementFilters() { return m_UIElementFilters; }
	// Get all filters
	Vector<String>& GetUIAllFilters() { return m_UIAllFilters; }
	// Get script filters
	Vector<String>& GetUIScriptFilters() { return m_UIScriptFilters; }
	// Get particle filters
	Vector<String>& GetUIParticleFilters() { return m_UIParticleFilters; }
	// Get renderpath filters
	Vector<String>& GetUIRenderPathFilters() { return m_UIRenderPathFilters; }

	// Get scene filter(extension)
	unsigned int& GetUISceneFilter() { return m_UISceneFilter; }
	// Get element filter(extension)
	unsigned int& GetUIElementFilter() { return m_UIElementFilter; }
	// Get node filter(extension)
	unsigned int& GetUINodeFilter() { return m_UINodeFilter; }
	// Get import filter(extension)
	unsigned int& GetUIImportFilter() { return m_UIScriptFilter; }
	// Get script filter(extension)
	unsigned int& GetUIScriptFilter() { return m_UIScriptFilter; }
	// Get particle filter(extension)
	unsigned int& GetUIParticleFilter() { return m_UIParticleFilter; }
	// Get renderPath filter(extension)
	unsigned int& GetUIRenderPathFilter() { return m_UIRenderPathFilter; }

	// Get scene path
	String& GetUIScenePath() { return m_UIScenePath; }
	// Get element path
	String& GetUIElementPath() { return  m_UIElementPath; }
	// Get node path
	String& GetUINodePath() { return  m_UINodePath; }
	// Get import path
	String& GetUIImportPath() { return  m_UIImportPath; }
	// Get script path
	String& GetUIScriptPath() { return m_UIScriptPath; }
	// Get particle path
	String& GetUIParticlePath() { return m_UIParticlePath; }
	// Get renderPath path
	String& GetUIRenderPathPath() { return m_UIRenderPathPath; }
	// Get program dir path
	String& GetProgramDirPath() { return m_ProgramDir; }
	// Get recentScenes path
	Vector<String>& GetUIRecentScenesPath() { m_UIRecentScenes; }
	// Get screenshot's directory path
	String& GetScreenshotDirPath() { return m_ScreenshotDir; }

protected:
	// Editor
	// Editor* m_pEditor;;
	// Made a weak ptr Strong Ptr causes segfaults;
	WeakPtr<Editor> m_pEditor;

	// Editor's default XML style
	SharedPtr<XMLFile> m_pEditorDefaultStyle;
	// Editor's icon XML style
	SharedPtr<XMLFile> m_pEditorIconStyle;
	// Editor's global variables (Node or UIElement hash-to-varname reverse mapping)
	VariantMap m_GlobalVarNames;

	// Editor' scene
    SharedPtr<Scene> m_pScene;
	// Editor's UI root.
    SharedPtr<UIElement> m_pRootUI;

    SharedPtr<ViewSettings> m_pViewSettings;

	// Editor plugin handling
    Vector<SharedPtr<EditorPlugin>> m_EditorPlugins;

	// Scene filters(extensions)
	Vector<String> m_UISceneFilters;
	// Element filters(extensions)
	Vector<String> m_UIElementFilters;
	// All filters(extensions)
	Vector<String> m_UIAllFilters;
	// Script filters(extensions)
	Vector<String> m_UIScriptFilters;
	// Particle filters(extensions)
	Vector<String> m_UIParticleFilters;
	// RenderPath filters(extensions)
	Vector<String> m_UIRenderPathFilters;

	// Scene filter(extension)
	unsigned int m_UISceneFilter;
	// Element filter(extension)
	unsigned int  m_UIElementFilter;
	// Node filter(extension)
	unsigned int  m_UINodeFilter;
	// Import filter(extension)
	unsigned int  m_UIImportFilter;
	// Script filter(extension)
	unsigned int  m_UIScriptFilter;
	// Particle filter(extension)
	unsigned int  m_UIParticleFilter;
	// RenderPath filter(extension)
	unsigned int  m_UIRenderPathFilter;

	// Scene path
	String m_UIScenePath;
	// Element path
	String m_UIElementPath;
	// Node path
	String m_UINodePath;
	// Import path
	String m_UIImportPath;
	// Script path
	String m_UIScriptPath;
	// Particle path
	String m_UIParticlePath;
	// RenderPath path
	String m_UIRenderPathPath;
	// Prorgram dir
	String m_ProgramDir;
	// RecentScenes path
	Vector<String> m_UIRecentScenes;
	// Screenshot's directory path
	String m_ScreenshotDir;
};

