#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "RightFrameImGuiUI.h"

bool RightFrameImGuiUI::initialized= false;
PanelTab RightFrameImGuiUI::tabNames[] = {{"Inspector"},
    {"ViewSettings"}
};

RightFrameImGuiUI::RightFrameImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

RightFrameImGuiUI::~RightFrameImGuiUI()
{
    //dtor
}


bool * RightFrameImGuiUI::ShowRightFrame(void)
{
    // return p_opened;
    bool * p_opened = NULL;

    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flags
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    // create    window
    ImGui::Begin("RightFrame",  p_opened, ImVec2(200,Height-8-128), 0.5, window_flags  );

    // if initialized
    if(!initialized)
    {
        // Set window position
        ImGui::SetWindowPos(ImVec2(Width-208,128));
    }

    static const char* tabNames[] = {"Inspector","View Settings"};
    static const int numTabs = sizeof(tabNames)/sizeof(tabNames[0]);
    static const char* tabTooltips[numTabs] = {"Render Tab Tooltip",""};
    static int selectedTab = 0;
    static int optionalHoveredTab = 0;
    /*const bool tabSelectedChanged =*/ImGui::TabLabels(numTabs,tabNames,selectedTab,tabTooltips,true,&optionalHoveredTab);
    ImGui::Text("\nTab Page For Tab: \"%s\" here.\n",tabNames[selectedTab]);
    if (optionalHoveredTab>=0) ImGui::Text("Mouse is hovering Tab Label: \"%s\".\n\n",tabNames[optionalHoveredTab]);

    if(selectedTab==1)
    {
        // camera movement speed
        ShowSettingsPanel();
    }

    ImGui::End();

    initialized = true;


    return p_opened;
}

void  RightFrameImGuiUI::ShowSettingsPanel(void)
{
    // test
    ImGui::SetCursorPosY(96);

    // Create two columns
    ImGui::Columns(2,NULL,false);

    // float test
    static float CameraBaseSpeed = .5f;
    static float CameraRotationSpeed = .5f;
    static float CameraShiftSpeedMultiplier = .5f;
    static float ViewNearClip=0.5f;
    static float ViewFarClip=0.5f;
    static float ViewFov=0.5f;
    static float StepMove=0.5f;
    static float StepRotate=0.5f;
    static float StepScale=0.5f;
    static float SnapScale=0.5f;


    ImGui::Text("Cam Base Speed");
    ImGui::NextColumn();
    ImGui::InputFloat("###CameraBaseSpeed_", &CameraBaseSpeed, 0.01f, 0.05f, -1);
    ImGui::NextColumn();

    ImGui::Text("Cam Rotation Speed");
    ImGui::NextColumn();
    ImGui::InputFloat("###CameraRotationSpeed_", &CameraRotationSpeed, 0.01f, 0.05f, -1);
    ImGui::NextColumn();

    ImGui::Text("Cam Shift Speed(X)");
    ImGui::NextColumn();
    ImGui::InputFloat("###CameraShiftSpeedMultiplier_", &CameraShiftSpeedMultiplier, 0.01f, 0.05f, -1);
    ImGui::NextColumn();

    //camera viewport
    ImGui::Text("View NearClip");
    ImGui::NextColumn();
    ImGui::InputFloat("###ViewNearClip_", &ViewNearClip, 0.01f, 0.05f, -1);
    ImGui::NextColumn();
    ImGui::Text("View FarClip");
    ImGui::NextColumn();
    ImGui::InputFloat("###ViewFarClip_", &ViewFarClip, 0.01f, 0.05f, -1);
    ImGui::NextColumn();
    ImGui::Text("View Fov");
    ImGui::NextColumn();
    ImGui::InputFloat("###ViewFov_", &ViewFov, 0.01f, 0.05f, -1);
    ImGui::NextColumn();

    // move steps
    ImGui::Text("Move Step");
    ImGui::NextColumn();
    ImGui::InputFloat("###StepMove_", &StepMove, 0.01f, 0.05f, -1);
    ImGui::NextColumn();
    ImGui::Text("Rotate Step");
    ImGui::NextColumn();
    ImGui::InputFloat("###StepRotate_", &StepRotate, 0.01f, 0.05f, -1);
    ImGui::NextColumn();
    ImGui::Text("Scale Step");
    ImGui::NextColumn();
    ImGui::InputFloat("###StepScale_", &StepScale, 0.01f, 0.05f, -1);
    ImGui::NextColumn();
    ImGui::Text("Snap Scale");
    ImGui::NextColumn();
    ImGui::InputFloat("###SnapScale_", &SnapScale, 0.01f, 0.05f, -1);
    ImGui::NextColumn();

    return;
}
