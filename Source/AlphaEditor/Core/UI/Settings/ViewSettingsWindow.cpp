#include "AlphaEditorStd.h"


#include "../../Editor/Settings/ViewSettings.h"

#include "ViewSettingsWindow.h"

#include "../UIGlobals.h"
#include "../UIUtils.h"


using namespace Urho3D;

class ViewSettings;

ViewSettingsWindow::ViewSettingsWindow(Context* context) : Window(context)
    ,m_settingsinit(false)
{
    // Create settings turn off resizing
    SetLayout(LM_VERTICAL, 4, IntRect(6 ,6, 6, 6));
    SetResizeBorder(IntRect(6, 6, 6, 6));
    SetResizable(true);
    SetName("ViewSettingsWindow");
    SetLayoutMode(LM_VERTICAL);
    SetAlignment(HA_LEFT,VA_TOP);

    SetModalFrameColor(Color(1,1,1,1));

    // Create Title Bar
    titleBar_ = CreateChild<UIElement>("VSW_TitleBar");
    titleBar_->SetInternal(true);
    titleBar_->SetFixedHeight(16);
    titleBar_->SetLayoutMode(LM_HORIZONTAL);

    titleText_ = titleBar_->CreateChild<Text>("VSW_TitleText");
    titleText_->SetInternal(true);
    titleText_->SetText("View Settings Window");

    closeButton_ = titleBar_->CreateChild<Button>("VSW_CloseButton");
    closeButton_->SetInternal(true);

    img_ = CreateChild<BorderImage>();
    img_->SetInternal(true);

    // Create container
    viewSettingsContainer_ = CreateChild<UIElement>("VSW_Container");
    viewSettingsContainer_->SetInternal(true);
    viewSettingsContainer_->SetLayoutMode(LM_VERTICAL);
    viewSettingsContainer_->SetAlignment(HA_LEFT,VA_TOP);

    // get count
    for(unsigned int i=0; i<10; i++)
    {
        // Create one child
        UIElement * thisContainer_ = viewSettingsContainer_->CreateChild<UIElement>("VSW_ContainerChild:"+String(i));

        thisContainer_->SetInternal(true);
        thisContainer_->SetFixedHeight(16);
        thisContainer_->SetFixedWidth(200);
        thisContainer_->SetLayoutMode(LM_HORIZONTAL);

        // create text
        Text * thisContainerText_ =   thisContainer_->CreateChild<Text>("VSW_ContainerChildText");
        thisContainerText_->SetInternal(true);

        // thisContainerText_->SetFont(Anonymous,12);          // temporary
        thisContainerText_->SetText("Setting");
        thisContainerText_->SetFixedWidth(120);
        thisContainerText_->SetLayoutSpacing(4);


        LineEdit * thisContainerSettingsEdit_ =   thisContainer_->CreateChild<LineEdit>("VSW_ContainerChildText:"+String(i));
        thisContainerSettingsEdit_->SetInternal(true);
        thisContainerSettingsEdit_->SetFixedWidth(40);

        // thisContainerText_->SetFont(Anonymous,12);          // temporary
        thisContainerSettingsEdit_->SetText("test");
        thisContainerSettingsEdit_->SetStyle("LineEdit");

        thisContainerSettingsEdit_->SetEditable(true);
        thisContainerSettingsEdit_->SetEnabled(true);

        SubscribeToEvent(thisContainerSettingsEdit_, E_TEXTFINISHED, URHO3D_HANDLER(ViewSettingsWindow, HandleSettingsChange));
        //SubscribeToEvent(thisContainerSettingsEdit_, E_TEXTCHANGED, URHO3D_HANDLER(ViewSettingsWindow, HandleSettingsChange));


    }
    return;
}

ViewSettingsWindow::~ViewSettingsWindow()
{

}

void ViewSettingsWindow::RegisterObject(Context* context)
{
    context->RegisterFactory<ViewSettingsWindow>();
    URHO3D_COPY_BASE_ATTRIBUTES(Window);

}


const String& ViewSettingsWindow::GetTitle()
{
    return titleText_->GetText();
}


UIElement* ViewSettingsWindow::GetTitleBar()
{
    return titleBar_;
}

void ViewSettingsWindow::CreateWindow()
{

    return;
}

void ViewSettingsWindow::SetScene(Scene* scene)
{

    scene_ = scene;
}

void ViewSettingsWindow::SetUIElement(UIElement* rootui)
{
    mainUI_ = rootui;
}




bool ViewSettingsWindow::UpdateSettings(ViewSettings * thisViewSettings)
{
    // set to true
    m_settingsinit=true;

    // set view settings pointer
    m_pViewSettings = thisViewSettings;

    // convert to variant map
    VariantMap viewSettings_ = m_pViewSettings->ToVariantMap();

    // set blank values
    UIElement * thisSetting = NULL;
    Text * thisSettingTitle = NULL;
    LineEdit * thisSettingValue = NULL;

    // Camera Base Speed
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:0",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Cam Speed");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["cameraBaseSpeed_"].GetFloat()));


    // Camera Rotation Speed
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:1",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Cam Rotation");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["cameraRotationSpeed_"].GetFloat()));

    // Camera Base Speed
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:2",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Cam Shift");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["cameraShiftSpeedMultiplier_"].GetFloat()));


    // Near Clip
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:3",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Near Clip");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["viewNearClip_"].GetFloat()));

    // Far Clip
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:4",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Far Clip");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["viewFarClip_"].GetFloat()));

    // Fov
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:5",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Fov");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["viewFov_"].GetFloat()));

    // Move Step
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:6",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Move Step");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["moveStep_"].GetFloat()));

    // Rotate Step
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:7",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Rotate Step");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["rotateStep_"].GetFloat()));

    // Scale Step
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:8",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Scale Step");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["scaleStep_"].GetFloat()));

    // Snap Scale
    thisSetting = viewSettingsContainer_->GetChild("VSW_ContainerChild:9",false);

    thisSettingTitle =(Text *) thisSetting->GetChild(0);
    thisSettingTitle->SetText("Snap Scale");

    thisSettingValue =(LineEdit *) thisSetting->GetChild(1);
    thisSettingValue->SetText(String(viewSettings_["snapScale_"].GetFloat()));

    return false;
}


void ViewSettingsWindow::HandleSettingsChange(StringHash eventType, VariantMap& eventData)
{
    // Get Data
    LineEdit * thisElement = (LineEdit *)eventData[TextFinished::P_ELEMENT].GetPtr();

        //if no element then return which should always be true
    if(!thisElement)
    {
        return;
    }

    // Convert float to number
    String thisString = eventData[TextFinished::P_TEXT].GetString();

    float thisFloat =  atof(thisString.CString());

    // this can directly change settings so the event might not be needed but the 3dview needs to know to reload the data so might as well set it here
    Vector<SettingMap> settingMap_;

    if(thisElement->GetName().Contains(":0"))
    {
        settingMap_.Push({String("cameraBaseSpeed_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":1"))
    {
        settingMap_.Push({String("cameraRotationSpeed_"),thisFloat});
    }
    else  if(thisElement->GetName().Contains(":2"))
    {

        settingMap_.Push({String("cameraShiftSpeedMultiplier_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":3"))
    {

        settingMap_.Push({String("viewNearClip_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":4"))
    {

        settingMap_.Push({String("viewFarClip_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":5"))
    {

        settingMap_.Push({String("viewFOV_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":6"))
    {

        settingMap_.Push({String("cameraBaseSpeed_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":7"))
    {

        settingMap_.Push({String("rotateStep_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":8"))
    {

        settingMap_.Push({String("scaleStep_"),thisFloat});
    }
    else if(thisElement->GetName().Contains(":9"))
    {

        settingMap_.Push({String("snapScale_"),thisFloat});
    }


    // do the conversion
    if(m_pViewSettings->SetFromVariantMap(settingMap_))
    {
        // if success send a event -should always be success ful
    }

    // send event to 3DPlugin to reload the settings

    return;
}


void ViewSettingsWindow::SetIconStyle(XMLFile* iconstyle)
{
    m_pIconStyle = iconstyle;
}
