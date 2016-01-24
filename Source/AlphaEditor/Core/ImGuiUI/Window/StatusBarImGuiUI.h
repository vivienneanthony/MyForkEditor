#ifndef STATUSBARIMGUIUI_H
#define STATUSBARIMGUIUI_H


class StatusBarImGuiUI : public Object
{

    URHO3D_OBJECT(StatusBarImGuiUI, Object);

    public:
        StatusBarImGuiUI(Context * context);
        virtual ~StatusBarImGuiUI();

        static bool * ShowStatusBar(String Output);

    protected:
    private:
        static bool initialized;
};

#endif // STATUSBARIMGUIUI_H
