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

#include "GameAssetSelector.h"


using namespace Urho3D;


GameAssetSelector::GameAssetSelector(Context* context) :
    Window(context)
{
    // Create Window
    SetLayout(LM_VERTICAL, 4, IntRect(6 ,6, 6, 6));
    SetResizeBorder(IntRect(6, 6, 6, 6));
    SetName("GameAssetSelector");
    SetLayoutMode(LM_VERTICAL);
    SetAlignment(HA_LEFT,VA_TOP);

    SetModalFrameColor(Color(1,1,1,1));

    //SetTexture(g_pApp->GetConstantResCache()->GetResource<Texture2D>("Textures/UI.png"));
    //SetImageRect(IntRect(48, 0, 60, 16));

    // Create title UIElement
    m_pTitleRegion = CreateChild <UIElement> ("GAS_TitleRegion");
    m_pTitleRegion->SetInternal(true);
    m_pTitleRegion->SetLayout(LM_HORIZONTAL);
    m_pTitleRegion->SetAlignment(HA_LEFT,VA_TOP);
    m_pTitleRegion->SetHeight(32);

    AddChild(m_pTitleRegion);

    // Add title and Close Button
    m_pTitleText = CreateChild <Text> ("GAS_TitleText");
    m_pTitleText->SetInternal(true);
    m_pTitleText -> SetText("Game Asset");

    m_pTitleRegion-> AddChild(m_pTitleText);

    m_pCloseButton = CreateChild <Button> ("GAS_CloseButton");
    m_pCloseButton->SetInternal(true);
    m_pTitleRegion -> AddChild(m_pCloseButton);


    // set color
    Color HoverColor(1.0f,0.5f,0.1f);
    Color SelectionColor(0.5f,0.1f,0.2f);
    Color ItemColor(1.0f,1.0f,1.0f);

    BorderImage * spacer = CreateChild<BorderImage>("GAS_Spacer");
    spacer->SetInternal(true);

    // Add List View
    m_pGameAssetListView = CreateChild <ListView> ("GAS_GameAssetListView");
    m_pGameAssetListView->SetInternal(true);
    m_pGameAssetListView->SetHighlightMode(HM_ALWAYS);
    m_pGameAssetListView->SetMultiselect(false);
    m_pGameAssetListView->SetName("GameAssetsListView");
    m_pGameAssetListView->SetColor(ItemColor);

    AddChild(m_pGameAssetListView);

    BorderImage * spacer2 = CreateChild<BorderImage>("GAS_Spacer2");
    spacer2->SetInternal(true);

    // Add A button region
    m_pButtonRegion = CreateChild <UIElement> ("GAS_ButtonRegion");
    m_pButtonRegion->SetLayout(LM_HORIZONTAL);
    m_pButtonRegion->SetInternal(true);

    AddChild(m_pButtonRegion); // Add spacer

    // Cancel button Add
    m_pCancelButton = CreateChild <Button> ("GAS_CancelButton");
    m_pCancelButton->SetInternal(true);
    m_pCancelButton->SetSize(60,24);

    Text * cancelButtonText = CreateChild <Text> ("GAS_CancelButtonText");
    cancelButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    cancelButtonText->SetInternal(true);
    cancelButtonText->SetText("Cancel");
    m_pCancelButton->AddChild(cancelButtonText);

    m_pButtonRegion->AddChild(m_pCancelButton);

    // Ok Button
    m_pOkButton = CreateChild <Button> ("GAS_OkButton");
    m_pOkButton ->SetInternal(true);

    Text * okButtonText = CreateChild <Text> ("GAS_OkButtonText");
    okButtonText->SetAlignment(HA_CENTER, VA_CENTER);
    okButtonText->SetInternal(true);
    okButtonText->SetText("Ok");
    m_pOkButton->AddChild(okButtonText);

    m_pButtonRegion->AddChild(m_pOkButton);

    AddChild(m_pButtonRegion);

    m_pOkButton->SetPosition(0,100);


    return;
}

GameAssetSelector::~GameAssetSelector()
{
    //Remove();
}

void GameAssetSelector::UpdateGameAssetsList(void)
{
    // Get File Sysstem
    FileSystem * currentFileSystem = g_pApp->GetFileSystem();

    // String Results
    Vector<String> dirs;

    // Clear
    gameAssetResource.Empty();

    // Get filesystem directory
    String currentPath = currentFileSystem->GetProgramDir()+"GameData/GameAssets/";

    // Get current path for all xml files
    currentFileSystem->ScanDir(dirs, currentPath, "*", SCAN_DIRS, true);

    if(dirs.Size())
    {
        for(unsigned int currentDir=0; currentDir<dirs.Size(); currentDir++)
        {
            // Vector <String of files
            Vector <String> dirFiles;

            // remove garbage directories
            if (dirs.At(currentDir).EndsWith("."))
                continue;

            currentFileSystem->ScanDir(dirFiles, currentPath+dirs.At(currentDir)+"/", "*.xml", SCAN_FILES, false);

            String fullPath = currentPath+dirs.At(currentDir)+"/";

            if(dirFiles.Size())
            {
                // loop through each file listing
                for(unsigned int fileListing = 0; fileListing<dirFiles.Size(); fileListing++)
                {
                    if(dirFiles.At(fileListing).Contains("Physics")||dirFiles.At(fileListing).Contains("Chemistry"))
                        continue;

                    gameAssetResource.Push(fullPath+dirFiles.At(fileListing));
                }
            }
        }
    }

    // set color - not sure about the background
    Color HoverColor(1.0f,0.5f,0.1f);
    Color SelectionColor(0.5f,0.1f,0.2f);
    Color ItemColor(1.0f,1.0f,1.0f);

    // Get the name
    String resourceName;


    //  loop through  each - no way to tell if it is part of something yet
    for(unsigned int i=0; i<gameAssetResource.Size(); i++)
    {
        /// Flag file for loading and load
        File loadResourceFile(context_, gameAssetResource.At(i), FILE_READ);

        XMLFile * resourceXML = new XMLFile(context_);

        resourceXML -> Load(loadResourceFile);

        XMLElement rootElem = resourceXML->GetRoot();

        /// If no configuration is set or no root
        if (rootElem.IsNull())
            continue;

        if (rootElem.HasAttribute("type"))
        {
            resourceName=rootElem.GetAttribute("type");
        }
        else
        {
            continue;
        }


        Text * NewItem = m_pGameAssetListView->CreateChild<Text>("GAS_Text");

        // Turn off internal
        NewItem->SetInternal(false);

        NewItem->SetText(resourceName);

        // Strip XMl
        m_pGameAssetListView->AddItem(NewItem);

        NewItem->SetColor(ItemColor);
        NewItem->SetHoverColor(HoverColor);
        NewItem->SetSelectionColor(SelectionColor);

        NewItem->SetStyle("Text");
    }
}


void GameAssetSelector::HandleOKPressed(StringHash eventType, VariantMap& eventData)
{
    return;
}

void GameAssetSelector::HandleCancelPressed(StringHash eventType, VariantMap& eventData)
{
    return;
}

void GameAssetSelector::RegisterObject(Context* context)
{
    context->RegisterFactory<GameAssetSelector>();
    URHO3D_COPY_BASE_ATTRIBUTES(Window);

}
