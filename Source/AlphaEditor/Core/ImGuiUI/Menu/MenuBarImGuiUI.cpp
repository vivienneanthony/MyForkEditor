#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "MenuBarImGuiUI.h"

MenuBarImGuiUI::MenuBarImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext()) :
    Object(context)
{
    //ctor
}

MenuBarImGuiUI::~MenuBarImGuiUI()
{
    //dtor
}

void MenuBarImGuiUI::ShowMenuBar(void)
{
    // Setup MenuBar
    if (ImGui::BeginMainMenuBar())
    {
        // Set Main Menu (Quit)
        if (ImGui::BeginMenu("AlphaEditor"))
        {
            ImGui::EndMenu();
        }
        // Set File Menu (File Save, Load, Export)
        if (ImGui::BeginMenu("File"))
        {

            ImGui::EndMenu();
        }
        // Set Create Menu (Game Asset)
        if (ImGui::BeginMenu("Create"))
        {

            ImGui::EndMenu();
        }
        // Set Tools Menu (Material Editor For example)
        if (ImGui::BeginMenu("Tools"))
        {

            ImGui::EndMenu();
        }
        // Set Preferences Menu (Configuration)
        if (ImGui::BeginMenu("Preferences"))
        {

            ImGui::EndMenu();
        }
        // Set About Menu (About Alpha, Team, Contact)
        if (ImGui::BeginMenu("About"))
        {

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}



