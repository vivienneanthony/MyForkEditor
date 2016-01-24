#pragma once



class MainToolBarImGuiUI : public Object
{
    URHO3D_OBJECT(MainToolBarImGuiUI, Object)
public:
    MainToolBarImGuiUI(Context * context);
    virtual ~MainToolBarImGuiUI();

    static bool * ShowToolBar(const unsigned int & SetToolBar, const Vector<ToolBarRow> & SetToolBars);
  //  static void SetToolBar(const Vector<ToolBarRow> SetToolBars);


protected:

private:

    //static Vector<ToolBarRow> ToolBars;
};



//Vector<ToolBarRow> MainToolBarImGuiUI::ToolBars;
