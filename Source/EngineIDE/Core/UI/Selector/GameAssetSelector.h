#pragma once


using namespace Urho3D;

class GameAssetSelector: public Window
{
    URHO3D_OBJECT(GameAssetSelector, Window);

public:

    GameAssetSelector(Context* context);
    virtual  ~GameAssetSelector();

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


    Button * GetCancelButton(void)
    {
        if(m_pCancelButton!=NULL) return m_pCancelButton;
        else return NULL;
    };

    ListView* GetListView(void)
    {
        if(m_pGameAssetListView!=NULL) return m_pGameAssetListView;

        else return NULL;
    };

    String GetResourceSelected(void)
    {
        return gameAssetResource.At(m_pGameAssetListView->GetSelection());
    };

private:

    void HandleOKPressed(StringHash eventType, VariantMap& eventData);
    void HandleCancelPressed(StringHash eventType, VariantMap& eventData);

// UI Elements
    SharedPtr<Window> m_pGameAssetSelectorWindow;
    SharedPtr<UIElement> m_pTitleRegion;
    SharedPtr<Text>m_pTitleText;
    SharedPtr<Button>m_pCloseButton;
    SharedPtr<ListView> m_pGameAssetListView;
    SharedPtr<UIElement> m_pButtonRegion;
    SharedPtr<Button>m_pCancelButton;
    SharedPtr<Button>m_pOkButton;

// Current Path
    String WorkingPath;

// Style
    SharedPtr <EditorData> m_pEditorData;

    Vector<String> gameAssetResource;
};

