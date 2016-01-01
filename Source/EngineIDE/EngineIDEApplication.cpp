//
// EngineSTD - EngineIDE Development Interface
//
// Programmers :
// Viviennie Anthony
// Andrej Skorinko
//
// Vivienne Anthony - Copyright (c) 2008-2015
//

#include "EngineIDEStd.h"

#include "GameLogic/EditorGameLogic.h"

#include "EngineIDEApplication.h"


// Core editor headers
#include "Core/Editor/Editor.h"
#include "Core/Editor/Editor.h"
#include "Core/Editor/Selection/EditorSelection.h"
#include "Core/Editor/View/EditorView.h"
#include "Core/Editor/EditorData.h"
#include "Core/Editor/Plugin/Scene/EPScene3D.h"
#include "Core/Editor/Browser/ResourceBrowser.h"

// Core InputActionSystem headers
#include "Core/Input/InputActionSystem.h"

// Core headers - UI, Windows, Global, and Attributes
#include "Core/UI/Attribute/AttributeContainer.h"
#include "Core/UI/Attribute/AttributeInspector.h"
#include "Core/UI/Attribute/AttributeVariableEvents.h"
#include "Core/UI/Attribute/AttributeVariable.h"
#include "Core/UI/Selector/DirSelector.h"
#include "Core/UI/Hierarchy/HierarchyWindow.h"
#include "Core/UI/Menu/MenuBarUI.h"
#include "Core/UI/Menu/MiniToolBarUI.h"
#include "Core/UI/Modal/ModalWindow.h"
#include "Core/UI/Picker/ResourcePicker.h"
#include "Core/UI/Tab/TabWindow.h"
#include "Core/UI/Menu/ToolBarUI.h"
#include "Core/UI/UIGlobals.h"
#include "Core/UI/UIUtils.h"

#include "Core/Utils/Macros.h"
#include "Core/Utils/Helpers.h"

#include "Core/Editor/Settings/IDESettings.h"

#include "GameView/MainEditorView.h"

#include "Urho3D/AngelScript/APITemplates.h"

// Register old components
#include "EngineStd/GameAssetManager/Factory/Components/EngineCamera/GameAssetEngineCamera.h"
#include "EngineStd/GameAssetManager/Factory/Components/EngineLight/GameAssetEngineLight.h"
#include "EngineStd/GameAssetManager/Factory/Components/EngineSkybox/GameAssetEngineSkybox.h"
#include "EngineStd/GameAssetManager/Factory/Components/EngineObject/GameAssetEngineObject.h"


// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(EngineIDEApp)

EngineIDEApp::EngineIDEApp(Context* context) : EngineApp(context)
{
    g_pApp = this;
    m_bIsInit = false;
    m_pCurrentCursor = nullptr;
}

EngineIDEApp::~EngineIDEApp()
{

}


void EngineIDEApp::Setup()
{
    // Register IDESettings subsystem
    IDESettings::RegisterObject(context_);

    // Register the new IDE
    IDESettings * m_settings = new IDESettings(context_);

    m_settings->LoadConfigFile();

    engineParameters_ = m_settings->ToVariantMap();

    EngineApp::Setup();

}

void EngineIDEApp::Start()
{
    // Register Components
    RegisterAllComponents();

    // Start the app
    EngineApp::Start();

    return;
}

void EngineIDEApp::Stop()
{

    EngineApp::Stop();

    return;
}

bool EngineIDEApp::CreateCursor()
{
    EngineApp::CreateCursor();

    return true;
}

bool EngineIDEApp::VCreateViewLogic()
{
    m_pGameLogic = new EditorGameLogic(context_);
    context_->RegisterSubsystem(m_pGameLogic);

    if (!m_pGameLogic->VInitialize())
    {
        URHO3D_LOGERROR("Game logic init failed");
        m_bIsInit = false;
    }

    // Game Logic Initialized
    URHO3D_LOGINFO("Game logic successfully initialized");

    // Create a game for for the Editor
    SharedPtr<IGameView> EditorView = SharedPtr<IGameView>(new MainEditorView(context_, m_pRenderer , true));
    m_pGameLogic->VAddView(EditorView);

    return true;
}

void EngineIDEApp::VInitializeAllDelegates()
{
    EngineApp::VInitializeAllDelegates();

    return;
}

// Destroy delegates
void EngineIDEApp::VDestroyAllDelegates()
{
    EngineApp::VDestroyAllDelegates();

    return;
}

// Register all components
void EngineIDEApp::RegisterAllComponents()
{
    //// Register all objects
    //IDESettings::RegisterObject(context_);
    //Editor::RegisterObject(context_);
    //ResourcePickerManager::RegisterObject(context_);
    //EditorData::RegisterObject(context_);
    //EditorView::RegisterObject(context_);
    //EditorSelection::RegisterObject(context_);
    //EPScene3DView::RegisterObject(context_);

    //// UI Related Objects
    //MenuBarUI::RegisterObject(context_);
    //ToolBarUI::RegisterObject(context_);
    //MiniToolBarUI::RegisterObject(context_);

    //// Window Related Objects
    //ModalWindow::RegisterObject(context_);
    //TabWindow::RegisterObject(context_);
    //HierarchyWindow::RegisterObject(context_);

	// Register old components
    GameAssetEngineLight::RegisterObject(context_);
    GameAssetEngineCamera::RegisterObject(context_);
    GameAssetEngineObject::RegisterObject(context_);
    GameAssetEngineSkybox::RegisterObject(context_);

    return;
}




