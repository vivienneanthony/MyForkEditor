#pragma once


using namespace Urho3D;

class AboutTeamGDPWindow: public Window
{
    URHO3D_OBJECT(AboutTeamGDPWindow, Window);

public:

    AboutTeamGDPWindow(Context* context);
    virtual  ~AboutTeamGDPWindow();

    static void RegisterObject(Context * context);
    void UpdateGameAssetsList(void);

    Button * GetOkButton(void)
    {
        if(m_pOkButton!=NULL) return m_pOkButton;
        else return NULL;
    };

    Button * GetCloseButton(void)
    {
        if(m_pCloseButton!=NULL) return m_pCloseButton;
        else return NULL;
    };

private:
	// UI Elements
    SharedPtr<Window>       m_pWindow;
    SharedPtr<UIElement>    m_pTitleRegion;
    SharedPtr<Text>         m_pTitleText;
    SharedPtr<Button>       m_pCloseButton;
    SharedPtr<Text>         m_pAboutUs;
    SharedPtr<Button>       m_pOkButton;
    SharedPtr<UIElement>    m_pButtonRegion;

};

