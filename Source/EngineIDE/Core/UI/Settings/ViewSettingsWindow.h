#pragma once


#include "../../Editor/Settings/ViewSettings.h"

#include "../../Utils/Macros.h"
#include "../UIGlobals.h"


using namespace Urho3D;


/// \todo redirect Double/Click, SelectionChange  ... event
class ViewSettingsWindow : public Window
{
    URHO3D_OBJECT(ViewSettingsWindow, Window);
public:
    /// Construct.
    ViewSettingsWindow(Context* context);
    /// Destruct.
    virtual ~ViewSettingsWindow();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Update
    void SetTitleBarVisible(bool show);
    /// Setters
    void SetTitle(const String& title);
    void SetScene(Scene* scene);
    void SetUIElement(UIElement* rootui);
    void SetIconStyle(XMLFile* iconstyle);

    /// use this to update
    bool UpdateSettings(ViewSettings * thisViewSettings);

    void HandleSettingsChange(StringHash eventType, VariantMap& eventData);

    /// Getters
    const String&	GetTitle();
    Scene*			GetScene();
    UIElement*		GetTitleBar();

protected:
    /// Create Window
    void    CreateWindow();

    /// Update
    void			UpdateDirtyUI();

 SharedPtr<XMLFile>	styleFile_;

    /// UI Elements
    SharedPtr<Text>		titleText_;
    SharedPtr<UIElement>	titleBar_;
    SharedPtr<UIElement>	viewSettingsContainer_;
    SharedPtr<Button>   closeButton_;
    SharedPtr<BorderImage> img_;
    SharedPtr<UIElement>    toolBar_;

    Vector<SharedPtr<UIElement>>   ViewUIContainer;

  /// \todo use weakptr
    WeakPtr<Scene> scene_;
    WeakPtr<UIElement> mainUI_;

    SharedPtr<ViewSettings> m_pViewSettings;

    bool m_settingsinit;
};
