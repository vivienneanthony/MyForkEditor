#pragma once



class LeftFrameImGuiUI : public Object
{
    URHO3D_OBJECT(LeftFrameImGuiUI, Object);

public:
    LeftFrameImGuiUI(Context * context);
    virtual ~LeftFrameImGuiUI();

    static void ShowLeftFrame(void);

    static bool initialized;

protected:
private:
};



