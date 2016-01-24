#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "../../Editor/Editor.h"
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

void MenuBarImGuiUI::ShowMenuBar(const Vector<SubMenu> & EditorMenuBar)
{
    if(ImGui::BeginMainMenuBar())
    {
        for(unsigned int i=0; i<EditorMenuBar.Size(); i++)
        {
            SubMenu ThisSub = EditorMenuBar.At(i);

            // Set Main Menu (Quit)
            if (ImGui::BeginMenu(ThisSub.Options.At(0).MenuLabel.CString()))
            {
                for(unsigned int i=1; i<ThisSub.Options.Size(); i++)
                {
                    ImGui::MenuItem(ThisSub.Options.At(i).MenuLabel.CString(), NULL);
                }
                ImGui::EndMenu();
            }

        }
        ImGui::EndMainMenuBar();

    }






















    /*

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
    */
}



