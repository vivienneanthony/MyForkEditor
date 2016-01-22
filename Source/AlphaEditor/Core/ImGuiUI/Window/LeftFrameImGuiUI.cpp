#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/ThirdParty/ImGui/imgui_internal.h>

#include "LeftFrameImGuiUI.h"

bool LeftFrameImGuiUI::initialized= false;

LeftFrameImGuiUI::LeftFrameImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

LeftFrameImGuiUI::~LeftFrameImGuiUI()
{
    //dtor
}

void LeftFrameImGuiUI::ShowLeftFrame(void)
{
    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flags
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    // Create window
    ImGui::Begin("LeftFrame", NULL,  ImVec2(200,Height-8-128), 0.5,  window_flags);

    // set location if its not set
    if(!initialized)
    {
        // Set window position
        ImGui::SetWindowPos(ImVec2(8,128));
    }

    static const char* tabNames[] = {"Hierarchy","Resources"};
    static const int numTabs = sizeof(tabNames)/sizeof(tabNames[0]);
    static const char* tabTooltips[numTabs] = {"Render Tab Tooltip"};
    static int selectedTab = 0;
    static int optionalHoveredTab = 0;
    /*const bool tabSelectedChanged =*/ImGui::TabLabels(numTabs,tabNames,selectedTab,tabTooltips,true,&optionalHoveredTab);
    ImGui::Text("\nTab Page For Tab: \"%s\" here.\n",tabNames[selectedTab]);
    if (optionalHoveredTab>=0) ImGui::Text("Mouse is hovering Tab Label: \"%s\".\n\n",tabNames[optionalHoveredTab]);

    ImGui::SetCursorPosY(96);

    // Use Tree Format for Nodes
    if(ImGui::TreeNode("Outline"))
    {
        if(ImGui::TreeNode("Scene"))
        {
            // Scene
            for (int i = 0; i < 5; i++)
            {
                if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                {
                    ImGui::Text("StaticComponent");

                    ImGui::TreePop();
                }
            }
        ImGui::TreePop();
        }

        if(ImGui::TreeNode("UI"))
        {
            ImGui::TreePop();
        }
    }

    ImGui::TreePop();



// Close current ImGui Context
ImGui::End();

initialized=true;

return;
}
