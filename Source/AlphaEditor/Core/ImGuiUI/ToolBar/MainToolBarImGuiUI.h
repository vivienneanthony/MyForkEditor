#pragma once



class MainToolBarImGuiUI : public Object
{
    URHO3D_OBJECT(MainToolBarImGuiUI, Object)
    public:
        MainToolBarImGuiUI(Context * context);
        virtual ~MainToolBarImGuiUI();

        static bool * ShowToolBar();
    protected:
    private:

};



