#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/ThirdParty/ImGui/imgui_internal.h>


#include "CameraInfoImGuiUI.h"

bool CameraInfoImGuiUI::initialized= false;


CameraInfoImGuiUI::CameraInfoImGuiUI(Context * context  = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

CameraInfoImGuiUI::~CameraInfoImGuiUI()
{
    //dtor
}

void CameraInfoImGuiUI::ShowCameraInfo(const Node* NodeCamera=NULL)
{
    // if no node
    if(NodeCamera==NULL)
    {
        return;
    }

    Vector3 cameraPos = NodeCamera->GetPosition();

    String xText(cameraPos.x_);
    String yText(cameraPos.y_);
    String zText(cameraPos.z_);


    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar ;
    window_flags |= ImGuiWindowFlags_NoCollapse;


    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flag
    ImGui::Begin("CameraPosition", NULL, ImVec2(200,32),0.5f, window_flags);

    if(!initialized)
    {
        ImGui::SetWindowPos(ImVec2(8,Height-32));
    }

    String PositionInfo = String( "Camera Pos: " + xText + " " + yText + " " + zText);

    ImGui::Text(PositionInfo.CString());

    ImGui::End();

    initialized = true;


    return;
}
