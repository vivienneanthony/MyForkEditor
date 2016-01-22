#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "RightFrameImGuiUI.h"

bool RightFrameImGuiUI::initialized= false;

RightFrameImGuiUI::RightFrameImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

RightFrameImGuiUI::~RightFrameImGuiUI()
{
    //dtor
}


void RightFrameImGuiUI::ShowRightFrame(void)
{
    // create tool bar
    ImGuiWindowFlags window_flags = 0;


    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flags
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    // create    window
    ImGui::Begin("RightFrame",  NULL, ImVec2(200,Height-8-128), 0.5, window_flags  );


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


    // test
    ImGui::SetCursorPosY(96);

    // Create two columns
    ImGui::Columns(2,NULL,false);

    // First column is text
    ImGui::Text("Camera Speed");

    // Next Column
    ImGui::NextColumn();

    // float test
    static float bar = .5f;
    ImGui::InputFloat("", &bar, 0.01f, 0.05f, -1);

    ImGui::End();

    initialized = true;

    return;

}
