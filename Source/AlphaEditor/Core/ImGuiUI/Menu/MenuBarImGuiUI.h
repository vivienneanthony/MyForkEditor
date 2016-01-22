#ifndef MenuBarImGuiUI_H
#define MenuBarImGuiUI_H


class MenuBarImGuiUI : public Object
{
    URHO3D_OBJECT(MenuBarImGuiUI, Object)
    public:
        MenuBarImGuiUI(Context * context);
        virtual ~MenuBarImGuiUI();

        static void ShowMenuBar();

    protected:
    private:
};

#endif // MenuBarImGuiUI_H
