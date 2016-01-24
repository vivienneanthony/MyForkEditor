#pragma once


class MenuBarImGuiUI : public Object
{
    URHO3D_OBJECT(MenuBarImGuiUI, Object)
    public:
        MenuBarImGuiUI(Context * context);
        virtual ~MenuBarImGuiUI();

        static void ShowMenuBar(const Vector<SubMenu> & EditorMenuBar);

    protected:
    private:
};

