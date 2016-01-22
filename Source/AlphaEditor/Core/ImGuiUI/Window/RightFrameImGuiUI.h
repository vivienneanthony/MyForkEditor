#pragma once

class RightFrameImGuiUI : public Object
{
    URHO3D_OBJECT(RightFrameImGuiUI, Object);

    public:
        RightFrameImGuiUI(Context * context);
        virtual ~RightFrameImGuiUI();

        // Show Right Frame
        static void ShowRightFrame(void);

        static bool initialized;
    protected:
    private:
};


