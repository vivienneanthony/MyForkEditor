//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "EngineIDEStd.h"

#include "../../Editor/Editor.h"
#include "../../Editor/EditorData.h"

#include "AboutTeamGDPWindow.h"


using namespace Urho3D;


AboutTeamGDPWindow::AboutTeamGDPWindow(Context* context) :
    Window(context)
{
    // Create Window
    SetLayout(LM_VERTICAL, 4, IntRect(6 ,6, 6, 6));
    SetResizeBorder(IntRect(6, 6, 6, 6));
    SetName("AboutTeamGDPWindow");
    SetLayoutMode(LM_VERTICAL);
    SetAlignment(HA_LEFT,VA_TOP);

    SetModalFrameColor(Color(1,1,1,1));

    SetTexture(g_pApp->GetConstantResCache()->GetResource<Texture2D>("Textures/UI.png"));
    SetImageRect(IntRect(48, 0, 60, 16));

    // Bring to front
    SetBringToFront(true);
    SetBringToBack(true);

    SetFocusMode(FM_FOCUSABLE_DEFOCUSABLE);


    // Create title UIElement
    m_pTitleRegion = CreateChild <UIElement> ("ATGW_TitleRegion");
    m_pTitleRegion->SetInternal(true);
    m_pTitleRegion->SetLayout(LM_HORIZONTAL);
    m_pTitleRegion->SetAlignment(HA_LEFT,VA_TOP);
    m_pTitleRegion->SetHeight(32);

    AddChild(m_pTitleRegion);

    // Add title and Close Button
    m_pTitleText = CreateChild <Text> ("ATGW_TitleText");
    m_pTitleText->SetInternal(true);
    m_pTitleText -> SetText("About Us");

    m_pTitleRegion-> AddChild(m_pTitleText);

    m_pCloseButton = CreateChild <Button> ("ATGW_CloseButton");
    m_pCloseButton->SetInternal(true);
    m_pTitleRegion -> AddChild(m_pCloseButton);


    // set color
    Color HoverColor(1.0f,0.5f,0.1f);
    Color SelectionColor(0.5f,0.1f,0.2f);
    Color ItemColor(1.0f,1.0f,1.0f);

    BorderImage * spacer = CreateChild<BorderImage>("ATGW_Spacer");
    spacer->SetInternal(true);

    // Add List View
    m_pAboutUs = CreateChild <Text> ("ATGW_AboutUs");
    AddChild(m_pAboutUs);
    m_pAboutUs->SetInternal(true);

    m_pAboutUs->SetText("Vivienne Anthony (Programmer) - Andrej (Programmer)");


    BorderImage * spacer2 = CreateChild<BorderImage>("ATGW_Spacer2");
    spacer2->SetInternal(true);

    // Add A button region
    m_pButtonRegion = CreateChild <UIElement> ("ATGW_ButtonRegion");
    m_pButtonRegion->SetLayout(LM_HORIZONTAL);
    m_pButtonRegion->SetInternal(true);

    AddChild(m_pButtonRegion); // Add spacer

    // Ok Button
    m_pOkButton = CreateChild <Button> ("ATGW_OkButton");
    m_pOkButton ->SetInternal(true);

    Text * okButtonText = CreateChild <Text> ("ATGW_OkButtonText");
    okButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    okButtonText->SetInternal(true);
    okButtonText->SetText("Ok");
    m_pOkButton->AddChild(okButtonText);

    m_pButtonRegion->AddChild(m_pOkButton);

    AddChild(m_pButtonRegion);

    m_pOkButton->SetPosition(0,100);


    return;
}

AboutTeamGDPWindow::~AboutTeamGDPWindow()
{
    //Remove();
}

void AboutTeamGDPWindow::RegisterObject(Context* context)
{
    context->RegisterFactory<AboutTeamGDPWindow>();
    URHO3D_COPY_BASE_ATTRIBUTES(Window);

}
