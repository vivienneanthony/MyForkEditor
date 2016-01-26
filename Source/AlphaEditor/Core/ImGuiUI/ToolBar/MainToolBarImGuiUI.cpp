#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>

#include "../../Editor/Editor.h"

#include "MainToolBarImGuiUI.h"

static unsigned int currentToolBar;
//static Vector<ToolBarRow> ToolBars;


MainToolBarImGuiUI::MainToolBarImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)

{
    currentToolBar = 0;

    //ctor
}

MainToolBarImGuiUI::~MainToolBarImGuiUI()
{
    //dtor
}


bool * MainToolBarImGuiUI::ShowToolBar(const unsigned int & SetToolBar, const Vector<ToolBarRow> & SetToolBars )
{
    // null
    bool * p_opened = NULL;

    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    // set flags
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;

    // create    window
    ImGui::Begin("ToolBar", p_opened, ImVec2(1440,52), 0.5, window_flags  );

    // Set window position
    ImGui::SetWindowPos(ImVec2(0,18));


    // get default text width and height
    float tex_w = (float)ImGui::GetIO().Fonts->TexWidth;
    float tex_h = (float)ImGui::GetIO().Fonts->TexHeight;
    ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
    // get font
    ImFontAtlas* atlas = ImGui::GetIO().Fonts;

    ImFont* font = atlas->Fonts[1];

    ImGui::PushFont(font);


    for(unsigned int i=0; i<SetToolBars.At(SetToolBar).Options.Size(); i++)
    {
        if(i)
        {
            ImGui::SameLine();
        }

        // get letter
        const char * Assignment = SetToolBars.At(SetToolBar).Options.At(i).Assignment;
        bool isseparater = SetToolBars.At(SetToolBar).Options.At(i).Separator;

        if(!isseparater)
        {
            ImGui::SmallButton(Assignment);
        }
        else
        {
            // Add dummy item
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(8,8));

        }
    }


    ImGui::PopFont();

    ImGui::End();

    return p_opened;

}


/*    /// loop through
    for (int i = 0; i < 1; i++)
    {
        // push id for button
        ImGui::PushID(i);

        // default pixel padding
        int frame_padding = 2;     // -1 = uses default padding

        /// set alpha
        ImGui::ImageButton(tex_id, ImVec2(24,24), ImVec2(0,0), ImVec2(24.0f/tex_w,24/tex_h), frame_padding, ImColor(0,0,0,255));

        // pop id - NOT sure why it's needed
        ImGui::PopID();

        // put the button on the same line
        ImGui::SameLine();
    }
*/

