#pragma once


class editorData;

class LeftFrameImGuiUI : public Object
{
    URHO3D_OBJECT(LeftFrameImGuiUI, Object);

public:
    LeftFrameImGuiUI(Context * context);
    virtual ~LeftFrameImGuiUI();

    static bool * ShowLeftFrame(void);

    static void SetScene(Scene * pSetEditorScene);

protected:
private:
 static bool initialized;
 static Scene*  pEditorScene;

 static void GetNodeData(const Node * ParentNode) ;

 static void ShowResourcesView(void);
 static void ShowHierarchyView(void);
};



