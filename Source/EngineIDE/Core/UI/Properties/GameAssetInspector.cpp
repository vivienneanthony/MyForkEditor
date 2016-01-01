#include "EngineIDEStd.h"

#include "../UIGlobals.h"
#include "../UIUtils.h"

using namespace Urho3D;

#include "GameAssetInspector.h"

void GameAssetInspector::RegisterObject(Context* context)
{
    context->RegisterFactory<GameAssetInspector>();
}

GameAssetInspector::GameAssetInspector(Context* context) : Window(context)
{
// Set style file
    styleFile_ = g_pApp->GetConstantResCache()->GetResource<XMLFile>("UI/IDEStyle.xml");

    // for some reason it's needed
    SetDefaultStyle(styleFile_);

    SetLayout(LM_VERTICAL, 4, IntRect(6 ,6, 6, 6));
    SetResizeBorder(IntRect(6, 6, 6, 6));
    SetResizable(false);
    SetName("GameAssetInspectorWindow");
    SetLayoutMode(LM_VERTICAL);
    SetAlignment(HA_LEFT,VA_TOP);


    titleBar_ = CreateChild<UIElement>("GAPW_TitleBar");
    titleBar_->SetInternal(true);
    titleBar_->SetFixedHeight(16);
    titleBar_->SetLayoutMode(LM_HORIZONTAL);

    titleText_ = titleBar_->CreateChild<Text>("GAPW_TitleText");
    titleText_->SetInternal(true);
    titleText_->SetText("Game Asset Inspector");

    closeButton_ = titleBar_->CreateChild<Button>("GAPW_CloseButton");
    closeButton_->SetInternal(true);

    img_ = CreateChild<BorderImage>();
    img_->SetInternal(true);

    UIElement* toolBar_ = CreateChild<UIElement>("GAPW_ToolBar");
    toolBar_->SetInternal(true);
    toolBar_->SetFixedHeight(0);
    toolBar_->SetLayoutMode(LM_HORIZONTAL);
    toolBar_->SetLayoutSpacing(4);

    // Create container
    UIElement * inspectorContainer_ = CreateChild<UIElement>("GAPW_Container");
    inspectorContainer_->SetInternal(true);
    inspectorContainer_->SetLayoutMode(LM_VERTICAL);
    inspectorContainer_->SetAlignment(HA_LEFT,VA_TOP);


    return;
}

GameAssetInspector::~GameAssetInspector()
{
    // blank
}

void GameAssetInspector::Update(void)
{
    // blank
}


void GameAssetInspector::SetScene(Scene* scene)
{

    scene_ = scene;
}

void GameAssetInspector::SetUIElement(UIElement* rootui)
{
    mainUI_ = rootui;
}
