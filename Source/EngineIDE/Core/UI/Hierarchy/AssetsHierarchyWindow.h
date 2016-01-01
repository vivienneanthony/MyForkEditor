#pragma once

#include "../../Utils/Macros.h"
#include "../UIGlobals.h"

using namespace Urho3D;

/// \todo redirect Double/Click, SelectionChange  ... event
class AssetsHierarchyWindow : public Window
{
    URHO3D_OBJECT(AssetsHierarchyWindow, Window);
public:
    /// Construct.
    AssetsHierarchyWindow(Context* context);
    /// Destruct.
    virtual ~AssetsHierarchyWindow();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Update
    void UpdateAssetsHierarchyItem(Serializable* serializable, bool clear=false);
    void SetTitleBarVisible(bool show);
    /// Setters
    void SetTitle(const String& title);
    void EnableToolButtons(bool enable);
    void SetVisibleToolButtons(bool visible);
    void SetScene(Scene* scene);
    void SetUIElement(UIElement* rootui);
    void SetIconStyle(XMLFile* iconstyle);

    /// Getters
    const String&	GetTitle();
    unsigned int	GetListIndex(Serializable* serializable);
    unsigned int	GetComponentListIndex(Component* component);
    Scene*			GetScene();
    UIElement*		GetUIElement();
    XMLFile*		GetIconStyle();
    ListView*		GetAssetsHierarchyList();
    UIElement*		GetTitleBar();
    // Serializable Attributes
    U_PROPERTY_IMP_PASS_BY_REF(Color,normalTextColor_,NormalTextColor)
    U_PROPERTY_IMP_PASS_BY_REF( Color ,nonEditableTextColor_,NonEditableTextColor)
    U_PROPERTY_IMP_PASS_BY_REF(Color,modifiedTextColor_,ModifiedTextColor)
    U_PROPERTY_IMP_PASS_BY_REF(Color,nodeTextColor_,NodeTextColor)
    U_PROPERTY_IMP_PASS_BY_REF(Color,componentTextColor_,ComponentTextColor)
    U_PROPERTY_IMP(bool,showInternalUIElement_,ShowInternalUIElement)
    U_PROPERTY_IMP(bool,showTemporaryObject_,ShowTemporaryObject)
    U_PROPERTY_IMP(bool,suppressSceneChanges_,SuppressSceneChanges)
    U_PROPERTY_IMP(bool,suppressUIElementChanges_,SuppressUIElementChanges)

protected:
    void ClearListView();
    bool TestDragDrop(UIElement* source, UIElement* target, int& itemType);
    //void SetID(Text* text, Serializable* serializable, int itemType = ITEM_NONE);
    void SetID(Text* text, Serializable* serializable, int itemType = 0);
    void AddComponentItem(unsigned int compItemIndex, Component* component, UIElement* parentItem);

    /// Update
    unsigned int	UpdateAssetsHierarchyItem(unsigned int itemIndex, Serializable* serializable, UIElement* parentItem);
    void			UpdateAssetsHierarchyItemText(unsigned int itemIndex, bool iconEnabled, const String& textTitle = NO_CHANGE);

    void			UpdateDirtyUI();

    /// UI actions
    void HideAssetsHierarchyWindow(StringHash eventType, VariantMap& eventData);
    void ExpandCollapseAssetsHierarchy(StringHash eventType, VariantMap& eventData);
    void HandleAssetsHierarchyListSelectionChange(StringHash eventType, VariantMap& eventData);
    void HandleAssetsHierarchyListDoubleClick(StringHash eventType, VariantMap& eventData);
    void HandleDragDropTest(StringHash eventType, VariantMap& eventData);
    void HandleDragDropFinish(StringHash eventType, VariantMap& eventData);
    void HandleTemporaryChanged(StringHash eventType, VariantMap& eventData);

    /// Scene Events
    void HandleNodeAdded(StringHash eventType, VariantMap& eventData);
    void HandleNodeRemoved(StringHash eventType, VariantMap& eventData);
    void HandleComponentAdded(StringHash eventType, VariantMap& eventData);
    void HandleComponentRemoved(StringHash eventType, VariantMap& eventData);
    void HandleNodeNameChanged(StringHash eventType, VariantMap& eventData);
    void HandleNodeEnabledChanged(StringHash eventType, VariantMap& eventData);
    void HandleComponentEnabledChanged(StringHash eventType, VariantMap& eventData);

    /// UI Events
    void HandleUIElementNameChanged(StringHash eventType, VariantMap& eventData);
    void HandleUIElementVisibilityChanged(StringHash eventType, VariantMap& eventData);
    void HandleUIElementAttributeChanged(StringHash eventType, VariantMap& eventData);
    void HandleUIElementAdded(StringHash eventType, VariantMap& eventData);
    void HandleUIElementRemoved(StringHash eventType, VariantMap& eventData);

    // UI Attributes
    SharedPtr<Text>		titleText_;
    SharedPtr<Button>	closeButton_;
    SharedPtr<Button>	expandButton_;
    SharedPtr<Button>	collapseButton_;
    SharedPtr<CheckBox> allCheckBox_;
    SharedPtr<ListView> AssetsHierarchyList_;
    SharedPtr<UIElement>	titleBar_;
    SharedPtr<BorderImage>	img_;
    // Serializable Attributes
    SharedPtr<XMLFile> iconStyle_;
    // other Attributes
    PODVector<unsigned int> AssetsHierarchyUpdateSelections_;
    /// \todo use weakptr
    WeakPtr<Scene> scene_;
    WeakPtr<UIElement> mainUI_;

};
