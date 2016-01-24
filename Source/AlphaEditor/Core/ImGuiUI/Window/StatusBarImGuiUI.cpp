#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "StatusBarImGuiUI.h"


bool StatusBarImGuiUI::initialized= false;

StatusBarImGuiUI::StatusBarImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

StatusBarImGuiUI::~StatusBarImGuiUI()
{


    //dtor
}

bool * StatusBarImGuiUI::ShowStatusBar(String Output)
{
    // use this method for now
    bool * p_opened = NULL;

    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar ;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;

    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flag
    ImGui::Begin("StatusBar",  p_opened, ImVec2(Width-16,32),0.5f, window_flags);

    if(!initialized)
    {
        ImGui::SetWindowPos(ImVec2(8, 68));
    }

    ImGui::Text(Output.CString());

    ImGui::End();

    initialized = true;

    return p_opened;
}
