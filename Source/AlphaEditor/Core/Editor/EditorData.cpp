#include "AlphaEditorStd.h"

// Core data
#include "Editor.h"

#include "Settings/ViewSettings.h"

#include "EditorData.h"



#include "Plugin/EditorPlugin.h"

using namespace Urho3D;
using namespace ImGui;


EditorData::EditorData(Context* context, Editor* editor) : Object(context),
	m_pEditor(editor)
	,m_pEditorDefaultStyle(NULL)
	,m_pEditorIconStyle(NULL)
{
    FileSystem* fileSystem = GetSubsystem<FileSystem>();

    m_UISceneFilters.Push("*.xml");
    m_UISceneFilters.Push("*.bin");
    m_UISceneFilters.Push("*.*");
    m_UIElementFilters.Push("*.xml");
    m_UIAllFilters.Push("*.*");
    m_UIScriptFilters.Push("*.*");
    m_UIScriptFilters.Push("*.as");
    m_UIScriptFilters.Push("*.lua");
    m_UIParticleFilters.Push("*.xml");
    m_UIRenderPathFilters.Push("*.xml");
    m_UISceneFilter = 0;
    m_UIElementFilter = 0;
    m_UINodeFilter = 0;
    m_UIImportFilter = 0;
    m_UIScriptFilter = 0;
    m_UIParticleFilter = 0;
    m_UIRenderPathFilter = 0;
	m_ProgramDir = fileSystem->GetProgramDir();
    m_UIScenePath = fileSystem->GetProgramDir() + "Data/Scenes";
    m_UIElementPath = fileSystem->GetProgramDir() + "Data/m_UI";
    m_UINodePath = fileSystem->GetProgramDir() + "Data/Objects";
    m_UIScriptPath = fileSystem->GetProgramDir() + "Data/Scripts";
    m_UIParticlePath = fileSystem->GetProgramDir() + "Data/Particles";
    m_UIRenderPathPath = fileSystem->GetProgramDir() + "CoreData/RenderPaths";
    m_ScreenshotDir = fileSystem->GetProgramDir() + "Screenshots";
}

EditorData::~EditorData()
{

}

void EditorData::RegisterObject(Context* context)
{
    context->RegisterFactory<EditorData>();
}

bool EditorData::Load(Editor* editor)
{
    ResourceCache* cache = g_pApp->GetConstantResCache();
    UI* ui_ = g_pApp->GetSubsystem<UI>();

    m_pEditorDefaultStyle = cache->GetResource<XMLFile>(editor->GetEditorDefaultStylePath());
    m_pEditorIconStyle = cache->GetResource<XMLFile>(editor->GetEditorIconStylePath());

	if (m_pEditorDefaultStyle && m_pEditorIconStyle)
	{
		m_pRootUI = ui_->GetRoot()->CreateChild<UIElement>("EditorRoot");
		m_pRootUI->SetSize(ui_->GetRoot()->GetSize());
		m_pRootUI->SetTraversalMode(TM_DEPTH_FIRST);    // This is needed for root-like element to prevent artifacts
		m_pRootUI->SetDefaultStyle(m_pEditorDefaultStyle);
		m_pRootUI->SetLayout(LM_VERTICAL);
	}
	else
	{
		URHO3D_LOGDEBUG("Failed to load editor XML files.");
		return false;
	}

    // if view settings exist else create one and reset to default
    if(editor->GetViewSettings())
    {
        m_pViewSettings=editor->GetViewSettings();
    }
    else
    {
        m_pViewSettings= new ViewSettings(context_);
    }



	return true;
}

bool EditorData::Load()
{
	if (!Load(m_pEditor))
	{
		return false;
	}

	return true;
}


