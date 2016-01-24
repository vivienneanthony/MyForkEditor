#pragma once


// struct for panels
 struct PanelTab
{
    const char *tabName;
    //void (*handler)(void);
};

class RightFrameImGuiUI : public Object
{
    URHO3D_OBJECT(RightFrameImGuiUI, Object);

public:
    RightFrameImGuiUI(Context * context);
    virtual ~RightFrameImGuiUI();

    // Show Right Frame
    static bool * ShowRightFrame(void);


protected:
private:
    static bool initialized;
    static PanelTab tabNames[];

    static void  ShowSettingsPanel(void);
};




