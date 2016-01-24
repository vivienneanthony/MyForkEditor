#pragma once


class editorData;

class LeftFrameImGuiUI : public Object
{
    URHO3D_OBJECT(LeftFrameImGuiUI, Object);

public:
    LeftFrameImGuiUI(Context * context);
    virtual ~LeftFrameImGuiUI();

    static bool * ShowLeftFrame(void);

    static void SetEditorData(EditorData * pSetEditorData);

protected:
private:
 static bool initialized;
 static EditorData *  pEditorData;

 static void GetNodeData(Node * ParentNode);
};



