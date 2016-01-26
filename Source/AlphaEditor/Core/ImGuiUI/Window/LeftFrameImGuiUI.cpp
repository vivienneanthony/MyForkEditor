#include <AlphaEngineStd.h>

#include <AlphaEngine/GameLogic/BaseGameLogic.h>

#include <AlphaEngine/ThirdParty/ImGui/imgui.h>
#include <AlphaEngine/ThirdParty/ImGui/imgui_internal.h>

#include "../../Editor/Editor.h"
#include "../../Editor/EditorData.h"

#include "LeftFrameImGuiUI.h"

bool LeftFrameImGuiUI::initialized= false;
Scene * LeftFrameImGuiUI::pEditorScene=NULL;

LeftFrameImGuiUI::LeftFrameImGuiUI(Context * context = g_pApp->GetGameLogic()->GetContext())
    :Object(context)
{
    //ctor
}

LeftFrameImGuiUI::~LeftFrameImGuiUI()
{
    //dtor
}

bool *  LeftFrameImGuiUI::ShowLeftFrame(void)
{
    // return p_opened
    bool * p_opened = NULL;

    // create tool bar
    ImGuiWindowFlags window_flags = 0;

    // get windows size
    unsigned int Width = g_pApp->GetGraphics()->GetWidth();
    unsigned int Height = g_pApp->GetGraphics()->GetHeight();

    // set flags
    window_flags |= ImGuiWindowFlags_NoTitleBar;

    // Create window
    ImGui::Begin("LeftFrame", p_opened,  ImVec2(200,Height-8-128), 0.5,  window_flags);

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


    if(selectedTab==0)
    {
        ShowHierarchyView();
    }

    if(selectedTab==1)
    {
        ShowResourcesView();
    }

    ImGui::TreePop();



// Close current ImGui Context
    ImGui::End();

    initialized=true;

    return p_opened;
}

void  LeftFrameImGuiUI::SetScene(Scene * pSetEditorScene)
{

    pEditorScene = pSetEditorScene;

    return;
}


void  LeftFrameImGuiUI::ShowHierarchyView(void)
{


    // Do the outline
    if(ImGui::TreeNode("Outline"))
    {
        // if editor data
        if(pEditorScene)
        {

            for(unsigned int i=0; i<pEditorScene->GetNumChildren(); i++)
            {
                GetNodeData(pEditorScene->GetChild(i));
            }
        }

        ImGui::TreePop();

    }

    return;
}



void LeftFrameImGuiUI::GetNodeData(const Node * ParentNode)
{

    // Create a unique name
    //String NodeName = ParentNode->GetName();
    //unsigned int NodeID= ParentNode->GetID();

    // Get the Node Id and Component Type
    String NameString = ParentNode->GetName()+String(":Id(")+String(ParentNode->GetID())+String(")");

    //ImVec2 pos = ImGui::GetCursorScreenPos();
    //ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
    //ImGui::RenderFrame(pos, ImVec2(pos.x + ImGui::GetContentRegionMax().x, pos.y + ImGui::GetTextLineHeight()), col, false);

    // assign a id
    NameString.Append(String("###Node_ID") + String(ParentNode->GetID())  );


    // Do the outline
    if(ImGui::TreeNode(NameString.CString()))
    {
        // Get Components
        const Vector< SharedPtr< Component > > & NodeComponents = ParentNode->GetComponents();

        for(unsigned int i=0; i < NodeComponents.Size(); i++)
        {
            String ComponentInfo = NodeComponents.At(i)->GetTypeName()+String(":Id(")+ String(NodeComponents.At(i)->GetID())+String(")");
            ImGui::Text(ComponentInfo.CString());
        }

        // for now just get the node
        for(unsigned int i=0; i <ParentNode->GetNumChildren(); i++)
        {
            GetNodeData(ParentNode->GetChild(i));
        }

        ImGui::TreePop();
    }

    return;
}




void LeftFrameImGuiUI::ShowResourcesView(void)
{

    ImGui::Text("Browser");

    ImGui::Dummy(ImVec2(8,8));

    const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
    static int listbox_item_current = 1;
    ImGui::ListBox("", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

    ImGui::Dummy(ImVec2(8,8));


    ImGui::SetCursorPosY(ImGui::GetCursorPosY()+32);

    ImGui::Separator();


    ImGui::Dummy(ImVec2(8,8));

    ImGui::Text("Preview");

}
