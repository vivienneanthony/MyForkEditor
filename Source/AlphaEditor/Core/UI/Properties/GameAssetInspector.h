#pragma once

#include "../../Utils/Macros.h"
#include "../UIGlobals.h"

using namespace Urho3D;

/// \todo redirect Double/Click, SelectionChange  ... event
class GameAssetInspector : public Window
{
    URHO3D_OBJECT(GameAssetInspector, Window);
public:
    /// Construct.
    GameAssetInspector(Context* context);
    /// Destruct.
    virtual ~GameAssetInspector();
    /// Register object factory.
    static void RegisterObject(Context* context);

    void SetScene(Scene* scene);
    void SetUIElement(UIElement* rootui);

    void SetIconStyle(XMLFile* iconstyle);

protected:
    void			Update();

    // UI Attributes
    SharedPtr<Text>		titleText_;
    SharedPtr<Button>	closeButton_;
    SharedPtr<UIElement>	titleBar_;
    SharedPtr<BorderImage>	img_;

    SharedPtr<XMLFile> m_pIconStyle;

    /// \todo use weakptr
    WeakPtr<Scene> scene_;
    WeakPtr<UIElement> mainUI_;

    SharedPtr<XMLFile>	styleFile_;
};
