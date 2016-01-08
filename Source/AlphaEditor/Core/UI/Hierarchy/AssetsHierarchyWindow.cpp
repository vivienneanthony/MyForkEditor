#include "AlphaEditorStd.h"

#include "AssetsHierarchyWindow.h"

#include "../UIGlobals.h"
#include "../UIUtils.h"

using namespace Urho3D;


AssetsHierarchyWindow::AssetsHierarchyWindow(Context* context) : Window(context)
{

    normalTextColor_ = Color(1.0f, 1.0f, 1.0f);
    modifiedTextColor_ = Color(1.0f, 0.8f, 0.5f);
    nonEditableTextColor_ = Color(0.7f, 0.7f, 0.7f);
    nodeTextColor_ = Color(1.0f, 1.0f, 1.0f);
    componentTextColor_ = Color(0.7f, 1.0f, 0.7f);
    showInternalUIElement_ = false;
    showTemporaryObject_ = false;
    suppressSceneChanges_ = false;
    suppressUIElementChanges_ = false;

    SetLayout(LM_VERTICAL, 4, IntRect(6 ,6, 6, 6));
    SetResizeBorder(IntRect(6, 6, 6, 6));
    SetResizable(true);
    SetName("AssetsHierarchyWindow");

    titleBar_ = CreateChild<UIElement>("AHW_TitleBar");
    titleBar_->SetInternal(true);
    titleBar_->SetFixedHeight(16);
    titleBar_->SetLayoutMode(LM_HORIZONTAL);

    titleText_ = titleBar_->CreateChild<Text>("AHW_TitleText");
    titleText_->SetInternal(true);
    titleText_->SetText("Assets Hierarchy Window");

    closeButton_ = titleBar_->CreateChild<Button>("AHW_CloseButton");
    closeButton_->SetInternal(true);

    img_ = CreateChild<BorderImage>();
    img_->SetInternal(true);

    UIElement* toolBar = CreateChild<UIElement>("AHW_ToolBar");
    toolBar->SetInternal(true);
    toolBar->SetFixedHeight(17);
    toolBar->SetLayoutMode(LM_HORIZONTAL);
    toolBar->SetLayoutSpacing(4);

    expandButton_ = toolBar->CreateChild<Button>("AHW_ExpandButton");
    expandButton_->SetInternal(true);
    expandButton_->SetFixedHeight(17);
    expandButton_->SetMinWidth(60);
    expandButton_->SetMaxWidth(70);
    expandButton_->SetLayoutMode(LM_HORIZONTAL);
    expandButton_->SetLayoutBorder(IntRect(1, 1, 1, 1));
    Text* label = expandButton_->CreateChild<Text>("AHW_ExpandText");
    label->SetInternal(true);
    label->SetText("Expand");

    collapseButton_ = toolBar->CreateChild<Button>("AHW_CollapseButton");
    collapseButton_->SetInternal(true);
    collapseButton_->SetFixedHeight(17);
    collapseButton_->SetMinWidth(60);
    collapseButton_->SetMaxWidth(70);
    collapseButton_->SetLayoutMode(LM_HORIZONTAL);
    collapseButton_->SetLayoutBorder(IntRect(1, 1, 1, 1));
    label = collapseButton_->CreateChild<Text>("AHW_CollapseText");
    label->SetInternal(true);
    label->SetText("Collapse");

    allCheckBox_ = toolBar->CreateChild<CheckBox>("AHW_AllCheckBox");
    allCheckBox_->SetInternal(true);
    allCheckBox_->SetFixedHeight(15);
    allCheckBox_->SetMinWidth(25);
    allCheckBox_->SetMaxWidth(45);
    allCheckBox_->SetLayoutMode(LM_HORIZONTAL);
    allCheckBox_->SetLayoutBorder(IntRect(5, 1, 1, 1));
    allCheckBox_->SetIndentSpacing(30);
    allCheckBox_->SetIndent(1);
    label = allCheckBox_->CreateChild<Text>("AHW_AllText");
    label->SetInternal(true);
    label->SetText("All");

    AssetsHierarchyList_ = CreateChild<ListView>("AHW_ListView");
    AssetsHierarchyList_->SetInternal(true);
    AssetsHierarchyList_->SetName("AssetsHierarchyList");
    AssetsHierarchyList_->SetHighlightMode(HM_ALWAYS);
    AssetsHierarchyList_->SetMultiselect(true);

    // Set selection to happen on click end, so that we can drag nodes to the inspector without resetting the inspector view
    AssetsHierarchyList_->SetSelectOnClickEnd(true);

    // Set drag & drop target mode on the node list background, which is used to parent nodes back to the root node
    AssetsHierarchyList_->GetContentElement()->SetDragDropMode(DD_TARGET);
    AssetsHierarchyList_->GetScrollPanel()->SetDragDropMode(DD_TARGET);

    SubscribeToEvent(closeButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, HideAssetsHierarchyWindow));
    SubscribeToEvent(expandButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, ExpandCollapseAssetsHierarchy));
    SubscribeToEvent(collapseButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, ExpandCollapseAssetsHierarchy));

    SubscribeToEvent(AssetsHierarchyList_, E_SELECTIONCHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleAssetsHierarchyListSelectionChange));
    SubscribeToEvent(AssetsHierarchyList_, E_ITEMDOUBLECLICKED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleAssetsHierarchyListDoubleClick));

    SubscribeToEvent(E_DRAGDROPTEST, URHO3D_HANDLER(AssetsHierarchyWindow, HandleDragDropTest));
    SubscribeToEvent(E_DRAGDROPFINISH, URHO3D_HANDLER(AssetsHierarchyWindow, HandleDragDropFinish));
    SubscribeToEvent(E_TEMPORARYCHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleTemporaryChanged));
}

AssetsHierarchyWindow::~AssetsHierarchyWindow()
{

}

void AssetsHierarchyWindow::RegisterObject(Context* context)
{
    context->RegisterFactory<AssetsHierarchyWindow>();
    URHO3D_COPY_BASE_ATTRIBUTES(Window);
    URHO3D_ACCESSOR_ATTRIBUTE("Suppress UIElement Changes", GetSuppressUIElementChanges, SetSuppressUIElementChanges, bool,false , AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Suppress Scene Changes", GetSuppressSceneChanges, SetSuppressSceneChanges, bool, false, AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Show Temporary Object", GetShowTemporaryObject, SetShowTemporaryObject, bool, false, AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Show Internal UIElement", GetShowInternalUIElement, SetShowInternalUIElement, bool, false, AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Component Text Color", GetComponentTextColor, SetComponentTextColor, Color, Color(0.7f, 1.0f, 0.7f), AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Node Text Color", GetNodeTextColor, SetNodeTextColor, Color, Color(1.0f, 1.0f, 1.0f), AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Modified Text Color", GetModifiedTextColor, SetModifiedTextColor, Color, Color(1.0f, 0.8f, 0.5f), AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Non Editable Text Color", GetNonEditableTextColor, SetNonEditableTextColor, Color, Color(0.7f, 0.7f, 0.7f), AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE("Normal Text Color", GetNormalTextColor, SetNormalTextColor, Color, Color(1.0f, 1.0f, 1.0f), AM_DEFAULT);

}

void AssetsHierarchyWindow::HideAssetsHierarchyWindow(StringHash eventType, VariantMap& eventData)
{
    SetVisible(false);
}

void AssetsHierarchyWindow::ExpandCollapseAssetsHierarchy(StringHash eventType, VariantMap& eventData)
{
    using namespace Released;

    Button* button = dynamic_cast<Button*>(eventData[P_ELEMENT].GetPtr());
    bool enable = button == expandButton_;

    bool all = allCheckBox_->IsChecked();
    allCheckBox_->SetChecked(false);    // Auto-reset

    const PODVector<unsigned int> selections = AssetsHierarchyList_->GetSelections();
    for (unsigned int i = 0; i < selections.Size(); ++i)
        AssetsHierarchyList_->Expand(selections[i], enable, all);
}

void AssetsHierarchyWindow::HandleAssetsHierarchyListSelectionChange(StringHash eventType, VariantMap& eventData)
{
    EnableToolButtons(AssetsHierarchyList_->GetSelections().Size() > 0);
    //editor_->OnAssetsHierarchyListSelectionChange(AssetsHierarchyList_->GetItems(), AssetsHierarchyList_->GetSelections());
}

void AssetsHierarchyWindow::HandleAssetsHierarchyListDoubleClick(StringHash eventType, VariantMap& eventData)
{
    using namespace ItemDoubleClicked;

// 		UIElement* item = dynamic_cast<UIElement*>(eventData[P_ITEM].GetPtr());
// 		editor_->OnAssetsHierarchyListDoubleClick(item);
}

void AssetsHierarchyWindow::HandleDragDropTest(StringHash eventType, VariantMap& eventData)
{
    using namespace DragDropTest;

// 		UIElement* source = dynamic_cast<UIElement*>(eventData[P_SOURCE].GetPtr());
// 		UIElement* target = dynamic_cast<UIElement*>(eventData[P_TARGET].GetPtr());
// 		int itemType;
// 		eventData[P_ACCEPT] = TestDragDrop(source, target, itemType);
}

void AssetsHierarchyWindow::HandleDragDropFinish(StringHash eventType, VariantMap& eventData)
{
    using namespace DragDropFinish;
}

void AssetsHierarchyWindow::HandleTemporaryChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace TemporaryChanged;
}

bool AssetsHierarchyWindow::TestDragDrop(UIElement* source, UIElement* target, int& itemType)
{

    return false;
}

void AssetsHierarchyWindow::EnableToolButtons(bool enable)
{
    expandButton_->SetEnabled(enable);
    collapseButton_->SetEnabled(enable);
    allCheckBox_->SetEnabled(enable);

    if (enable)
    {
        expandButton_->GetChild(0)->SetColor(normalTextColor_ );
        collapseButton_->GetChild(0)->SetColor(normalTextColor_);
        allCheckBox_->GetChild(0)->SetColor(normalTextColor_);
    }
    else
    {
        expandButton_->GetChild(0)->SetColor(nonEditableTextColor_);
        collapseButton_->GetChild(0)->SetColor(nonEditableTextColor_);
        allCheckBox_->GetChild(0)->SetColor(nonEditableTextColor_);
    }
}

void AssetsHierarchyWindow::SetVisibleToolButtons(bool visible)
{
    expandButton_->SetVisible(visible);
    collapseButton_->SetVisible(visible);
    allCheckBox_->SetVisible(visible);
    if (visible)
    {
        SubscribeToEvent(closeButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, HideAssetsHierarchyWindow));
        SubscribeToEvent(expandButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, ExpandCollapseAssetsHierarchy));
        SubscribeToEvent(collapseButton_, E_RELEASED, URHO3D_HANDLER(AssetsHierarchyWindow, ExpandCollapseAssetsHierarchy));
    }
    else
    {
        UnsubscribeFromEvent(closeButton_, E_RELEASED);
        UnsubscribeFromEvent(expandButton_, E_RELEASED);
        UnsubscribeFromEvent(collapseButton_, E_RELEASED);
    }
}
void AssetsHierarchyWindow::HandleNodeAdded(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeAdded;

    if (suppressSceneChanges_)
        return;

    Node* node = dynamic_cast<Node*>(eventData[P_NODE].GetPtr());
    if (showTemporaryObject_ || !node->IsTemporary())
        UpdateAssetsHierarchyItem(node);
}

void AssetsHierarchyWindow::HandleNodeRemoved(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeRemoved;
    if (suppressSceneChanges_)
        return;
    Node* node = dynamic_cast<Node*>(eventData[P_NODE].GetPtr());
    unsigned int index = GetListIndex(node);
    UpdateAssetsHierarchyItem(index, NULL, NULL);
}

void AssetsHierarchyWindow::HandleComponentAdded(StringHash eventType, VariantMap& eventData)
{
    using namespace ComponentAdded;
    if (suppressSceneChanges_)
        return;
    // Insert the newly added component at last component position but before the first child node position of the parent node
    Node* node = dynamic_cast<Node*>(eventData[P_NODE].GetPtr());
    Component* component = dynamic_cast<Component*>(eventData[P_COMPONENT].GetPtr());

    if (showTemporaryObject_ || !component->IsTemporary())
    {
        unsigned int nodeIndex = GetListIndex(node);
        if (nodeIndex != NO_ITEM)
        {
            unsigned int index = node->GetNumChildren() > 0 ? GetListIndex(node->GetChildren()[0]) : M_MAX_UNSIGNED;
            UpdateAssetsHierarchyItem(index, component, AssetsHierarchyList_->GetItems()[nodeIndex]);
        }
    }
}

void AssetsHierarchyWindow::HandleComponentRemoved(StringHash eventType, VariantMap& eventData)
{
    using namespace ComponentRemoved;
    if (suppressSceneChanges_)
        return;
    Component* component = dynamic_cast<Component*>(eventData[P_COMPONENT].GetPtr());
    unsigned int index = GetComponentListIndex(component);
    if (index != NO_ITEM)
        AssetsHierarchyList_->RemoveItem(index);
}

void AssetsHierarchyWindow::HandleNodeNameChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeNameChanged;

    if (suppressSceneChanges_)
        return;

    Node* node = dynamic_cast<Node*>(eventData[P_NODE].GetPtr());
    UpdateAssetsHierarchyItemText(GetListIndex(node), node->IsEnabled(), UIUtils::GetNodeTitle(node));
}

void AssetsHierarchyWindow::HandleNodeEnabledChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace NodeEnabledChanged;
    if (suppressSceneChanges_)
        return;

    Node* node = dynamic_cast<Node*>(eventData[P_NODE].GetPtr());
    UpdateAssetsHierarchyItemText(GetListIndex(node), node->IsEnabled());

}

void AssetsHierarchyWindow::HandleComponentEnabledChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace ComponentEnabledChanged;
    if (suppressSceneChanges_)
        return;

    Component* component = dynamic_cast<Component*>(eventData[P_COMPONENT].GetPtr());
    UpdateAssetsHierarchyItemText(GetComponentListIndex(component), component->IsEnabledEffective());

}

void AssetsHierarchyWindow::HandleUIElementNameChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace NameChanged;

    if (suppressUIElementChanges_)
        return;
    UIElement* element = dynamic_cast<UIElement*>(eventData[P_ELEMENT].GetPtr());
    UpdateAssetsHierarchyItemText(GetListIndex(element), element->IsVisible(), UIUtils::GetUIElementTitle(element));
}

void AssetsHierarchyWindow::HandleUIElementVisibilityChanged(StringHash eventType, VariantMap& eventData)
{
    using namespace VisibleChanged;
    if (suppressUIElementChanges_)
        return;
    UIElement* element = dynamic_cast<UIElement*>(eventData[P_ELEMENT].GetPtr());
    UpdateAssetsHierarchyItemText(GetListIndex(element), element->IsVisible());

}

void AssetsHierarchyWindow::HandleUIElementAttributeChanged(StringHash eventType, VariantMap& eventData)
{

    // Do not refresh the attribute inspector while the attribute is being edited via the attribute-editors
    if (suppressUIElementChanges_ )
        return;
    UIElement* element = dynamic_cast<UIElement*>(eventData[Positioned::P_ELEMENT].GetPtr());
// 		for (uint i = 0; i < editUIElements.length; ++i)
// 		{
// 			if (editUIElements[i] is element)
// 				attributesDirty = true;
// 		}
}

void AssetsHierarchyWindow::HandleUIElementAdded(StringHash eventType, VariantMap& eventData)
{
    using namespace ElementAdded;
    if (suppressUIElementChanges_)
        return;
    UIElement* element = dynamic_cast<UIElement*>(eventData[P_ELEMENT].GetPtr());
//*edit    if ((showInternalUIElement_ || !element->IsInternal()) && (showTemporaryObject_ || !element->IsTemporary()))
    //*edit UpdateAssetsHierarchyItem(element);
}

void AssetsHierarchyWindow::HandleUIElementRemoved(StringHash eventType, VariantMap& eventData)
{
    using namespace ElementRemoved;
    if (suppressUIElementChanges_)
        return;
    UIElement* element = (UIElement*)eventData[P_ELEMENT].GetPtr();
    UpdateAssetsHierarchyItem(GetListIndex(element), NULL, NULL);
}

void AssetsHierarchyWindow::UpdateAssetsHierarchyItemText(unsigned int itemIndex, bool iconEnabled, const String& textTitle /*= NO_CHANGE*/)
{
    Text* text = (Text*)AssetsHierarchyList_->GetItem(itemIndex);
    if (text == NULL)
        return;

    UIUtils::SetIconEnabledColor(text, iconEnabled);

    if (textTitle != NO_CHANGE)
        text->SetText(textTitle);
}

void AssetsHierarchyWindow::UpdateAssetsHierarchyItem(Serializable* serializable, bool clear)
{
    if (clear)
    {

        AssetsHierarchyList_->ClearSelection();
        /// \todo
        // Remove the current selection before updating the list item (in turn trigger an update on the attribute editor)
        // Clear copybuffer when whole window refreshed

    }

    // In case of item's parent is not found in the AssetsHierarchy list then the item will be inserted at the list root level
    Serializable* parent;
    switch (UIUtils::GetType(serializable))
    {
    case ITEM_NODE:
        parent = static_cast<Node*>(serializable)->GetParent();
        break;

    case ITEM_COMPONENT:
        parent = static_cast<Component*>(serializable)->GetNode();
        break;

    case ITEM_UI_ELEMENT:
        parent = static_cast<UIElement*>(serializable)->GetParent();
        break;

    default:
        break;
    }
    UIElement* parentItem = AssetsHierarchyList_->GetItem(GetListIndex(parent));
    UpdateAssetsHierarchyItem(GetListIndex(serializable), serializable, parentItem);
}

void AssetsHierarchyWindow::SetTitleBarVisible(bool show)
{
    img_->SetVisible(show);
    titleBar_->SetVisible(show);
}

void AssetsHierarchyWindow::ClearListView()
{
    AssetsHierarchyList_->RemoveAllItems();
}

void AssetsHierarchyWindow::SetTitle(const String& title)
{
    titleText_->SetText(title);
}

unsigned int AssetsHierarchyWindow::UpdateAssetsHierarchyItem(unsigned int itemIndex, Serializable* serializable, UIElement* parentItem)
{
    // Whenever we're updating, disable layout update to optimize speed
    AssetsHierarchyList_->GetContentElement()->DisableLayoutUpdate();

    if (serializable == NULL)
    {
        AssetsHierarchyList_->RemoveItem(itemIndex);
        AssetsHierarchyList_->GetContentElement()->EnableLayoutUpdate();
        AssetsHierarchyList_->GetContentElement()->UpdateLayout();
        return itemIndex;
    }

    int itemType = UIUtils::GetType(serializable);
    Variant id = UIUtils::GetID(serializable, itemType);

    // Remove old item if exists
    if (itemIndex < AssetsHierarchyList_->GetNumItems() && UIUtils::MatchID(AssetsHierarchyList_->GetItem(itemIndex), id, itemType))
        AssetsHierarchyList_->RemoveItem(itemIndex);

    Text* text = new Text(context_);
    AssetsHierarchyList_->InsertItem(itemIndex, text, parentItem);
    text->SetStyle("FileSelectorListText");

    if (serializable->GetType() == SCENE_TYPE || serializable == mainUI_.Get())
        // The root node (scene) and editor's root UIElement cannot be moved by drag and drop
        text->SetDragDropMode(DD_TARGET);
    else
        // Internal UIElement is not able to participate in drag and drop action
        text->SetDragDropMode(itemType == ITEM_UI_ELEMENT && static_cast<UIElement*>(serializable)->IsInternal() ? DD_DISABLED : DD_SOURCE_AND_TARGET);

    // Advance the index for the child items
    if (itemIndex == M_MAX_UNSIGNED)
        itemIndex = AssetsHierarchyList_->GetNumItems();
    else
        ++itemIndex;

    String iconType = serializable->GetTypeName();
    if (serializable == mainUI_.Get())
        iconType = "Root" + iconType;

    if (iconStyle_)
        UIUtils::IconizeUIElement(context_, iconStyle_,text, iconType);


    SetID(text, serializable, itemType);
    switch (itemType)
    {
    case ITEM_NODE:
    {
        Node* node = static_cast<Node*>(serializable);

        text->SetText(UIUtils::GetNodeTitle(node));
        text->SetColor(nodeTextColor_);
        UIUtils::SetIconEnabledColor(text, node->IsEnabled());

        // Update components first
        for (unsigned int i = 0; i < node->GetNumComponents(); ++i)
        {
            Component* component = node->GetComponents()[i];
            if (showTemporaryObject_ || !component->IsTemporary())
                AddComponentItem(itemIndex++, component, text);
        }

        // Then update child nodes recursively
        for (unsigned int i = 0; i < node->GetNumChildren(); ++i)
        {
            Node* childNode = node->GetChildren()[i];
            if (showTemporaryObject_ || !childNode->IsTemporary())
                itemIndex = UpdateAssetsHierarchyItem(itemIndex, childNode, text);
        }

        break;
    }

    case ITEM_COMPONENT:
    {
        Component* component = static_cast<Component*>(serializable);
        text->SetText(UIUtils::GetComponentTitle(component));
        text->SetColor(componentTextColor_);
        UIUtils::SetIconEnabledColor(text, component->IsEnabledEffective());
        break;
    }

    case ITEM_UI_ELEMENT:
    {
        UIElement* element = static_cast<UIElement*>(serializable);

        text->SetText(UIUtils::GetUIElementTitle(element));
        UIUtils::SetIconEnabledColor(text, element->IsVisible());

        // Update child elements recursively
        for (unsigned int i = 0; i < element->GetNumChildren(); ++i)
        {
            UIElement* childElement = element->GetChildren()[i];
            if ((showInternalUIElement_ || !childElement->IsInternal()) && (showTemporaryObject_ || !childElement->IsTemporary()))
                itemIndex = UpdateAssetsHierarchyItem(itemIndex, childElement, text);
        }

        break;
    }

    default:
        break;
    }

    // Re-enable layout update (and do manual layout) now
    AssetsHierarchyList_->GetContentElement()->EnableLayoutUpdate();
    AssetsHierarchyList_->GetContentElement()->UpdateLayout();

    return itemIndex;
}

void AssetsHierarchyWindow::UpdateDirtyUI()
{
    // Perform AssetsHierarchy selection latently after the new selections are finalized (used in undo/redo action)
    if (!AssetsHierarchyUpdateSelections_.Empty())
    {
        AssetsHierarchyList_->SetSelections(AssetsHierarchyUpdateSelections_);
        AssetsHierarchyUpdateSelections_.Clear();
    }
}

void AssetsHierarchyWindow::SetID(Text* text, Serializable* serializable, int itemType /*= ITEM_NONE*/)
{
    // If item type is not provided, auto detect it
    if (itemType == ITEM_NONE)
//        itemType = UIUtils::GetType(serializable);
        itemType = UIUtils::GetType(serializable);

    text->SetVar(TYPE_VAR, Variant(itemType));

    text->SetVar(ID_VARS[itemType], UIUtils::GetID(serializable, itemType));

    // Set node ID as drag and drop content for node ID editing
    if (itemType == ITEM_NODE)
        text->SetVar(DRAGDROPCONTENT_VAR, String(text->GetVar(NODE_ID_VAR).GetUInt()));

    switch (itemType)
    {
    case ITEM_COMPONENT:
        text->SetVar(NODE_ID_VAR, static_cast<Component*>(serializable)->GetNode()->GetID());
        break;

    case ITEM_UI_ELEMENT:
        // Subscribe to UI-element events
        SubscribeToEvent(serializable, E_NAMECHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementNameChanged));
        SubscribeToEvent(serializable, E_VISIBLECHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementVisibilityChanged));
        SubscribeToEvent(serializable, E_RESIZED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementAttributeChanged));
        SubscribeToEvent(serializable, E_POSITIONED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementAttributeChanged));
        break;

    default:
        break;
    }
}
void AssetsHierarchyWindow::SetScene(Scene* scene)
{
    if (scene != NULL)
    {
        UpdateAssetsHierarchyItem(scene);
        SubscribeToEvent(scene, E_NODEADDED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleNodeAdded));
        SubscribeToEvent(scene, E_NODEREMOVED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleNodeRemoved));
        SubscribeToEvent(scene, E_COMPONENTADDED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleComponentAdded));
        SubscribeToEvent(scene, E_COMPONENTREMOVED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleComponentRemoved));
        SubscribeToEvent(scene, E_NODENAMECHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleNodeNameChanged));
        SubscribeToEvent(scene, E_NODEENABLEDCHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleNodeEnabledChanged));
        SubscribeToEvent(scene, E_COMPONENTENABLEDCHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleComponentEnabledChanged));
    }
    else
    {
        UnsubscribeFromEvent(scene_, E_NODEADDED);
        UnsubscribeFromEvent(scene_, E_NODEREMOVED);
        UnsubscribeFromEvent(scene_, E_COMPONENTADDED);
        UnsubscribeFromEvent(scene_, E_COMPONENTREMOVED);
        UnsubscribeFromEvent(scene_, E_NODENAMECHANGED);
        UnsubscribeFromEvent(scene_, E_NODEENABLEDCHANGED);
        UnsubscribeFromEvent(scene_, E_COMPONENTENABLEDCHANGED);
        unsigned int index = GetListIndex(scene_);
        UpdateAssetsHierarchyItem(index, NULL, NULL);
    }
    scene_ = scene;
}

void AssetsHierarchyWindow::SetUIElement(UIElement* rootui)
{
    if (rootui != NULL)
    {
        UpdateAssetsHierarchyItem(rootui);
        // Subscribe to UI-element events
        SubscribeToEvent(rootui, E_NAMECHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementNameChanged));
        SubscribeToEvent(rootui, E_VISIBLECHANGED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementVisibilityChanged));
        SubscribeToEvent(rootui, E_RESIZED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementAttributeChanged));
        SubscribeToEvent(rootui, E_POSITIONED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementAttributeChanged));

        // This is needed to distinguish our own element events from Editor's UI element events
        rootui->SetElementEventSender(true);
        SubscribeToEvent(rootui, E_ELEMENTADDED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementAdded));
        SubscribeToEvent(rootui, E_ELEMENTREMOVED, URHO3D_HANDLER(AssetsHierarchyWindow, HandleUIElementRemoved));
    }
    else
    {
        if (mainUI_.Get())
            mainUI_->SetElementEventSender(false);
        UnsubscribeFromEvent(mainUI_, E_ELEMENTADDED);
        UnsubscribeFromEvent(mainUI_, E_ELEMENTREMOVED);
        UnsubscribeFromEvent(mainUI_, E_NAMECHANGED);
        UnsubscribeFromEvent(mainUI_, E_VISIBLECHANGED);
        UnsubscribeFromEvent(mainUI_, E_RESIZED);
        UnsubscribeFromEvent(mainUI_, E_POSITIONED);
        unsigned int index = GetListIndex(mainUI_);
        UpdateAssetsHierarchyItem(index, NULL, NULL);
    }
    mainUI_ = rootui;
}

void AssetsHierarchyWindow::SetIconStyle(XMLFile* iconstyle)
{
    iconStyle_ = iconstyle;
}

const String& AssetsHierarchyWindow::GetTitle()
{
    return titleText_->GetText();
}

unsigned int AssetsHierarchyWindow::GetListIndex(Serializable* serializable)
{
    if (serializable == NULL)
        return NO_ITEM;

    int itemType = UIUtils::GetType(serializable);
    Variant id = UIUtils::GetID(serializable, itemType);

    unsigned int numItems = AssetsHierarchyList_->GetNumItems();
    for (unsigned int i = 0; i < numItems; ++i)
    {
        if (UIUtils::MatchID(AssetsHierarchyList_->GetItem(i), id, itemType))
            return i;
    }

    return NO_ITEM;
}

unsigned int AssetsHierarchyWindow::GetComponentListIndex(Component* component)
{
    if (component == NULL)
        return NO_ITEM;

    unsigned int numItems = AssetsHierarchyList_->GetNumItems();
    for (unsigned int i = 0; i < numItems; ++i)
    {
        UIElement* item = AssetsHierarchyList_->GetItems()[i];
        if (item->GetVar(TYPE_VAR).GetInt() == ITEM_COMPONENT && item->GetVar(COMPONENT_ID_VAR).GetUInt() == component->GetID())
            return i;
    }

    return NO_ITEM;
}

Scene* AssetsHierarchyWindow::GetScene()
{
    return scene_;
}

UIElement* AssetsHierarchyWindow::GetUIElement()
{
    return mainUI_;
}

XMLFile* AssetsHierarchyWindow::GetIconStyle()
{
    return iconStyle_;
}

ListView* AssetsHierarchyWindow::GetAssetsHierarchyList()
{
    return AssetsHierarchyList_;
}

UIElement* AssetsHierarchyWindow::GetTitleBar()
{
    return titleBar_;
}

void AssetsHierarchyWindow::AddComponentItem(unsigned int compItemIndex, Component* component, UIElement* parentItem)
{
    Text* text = new Text(context_);
    AssetsHierarchyList_->InsertItem(compItemIndex, text, parentItem);
    text->SetStyle("FileSelectorListText");
    text->SetVar(TYPE_VAR, ITEM_COMPONENT);
    text->SetVar(NODE_ID_VAR, component->GetNode()->GetID());
    text->SetVar(COMPONENT_ID_VAR, component->GetID());
    text->SetText(UIUtils::GetComponentTitle(component));
    text->SetColor(componentTextColor_);
    // Components currently act only as drag targets
    text->SetDragDropMode(DD_TARGET);
    if (iconStyle_)
        UIUtils::IconizeUIElement(context_, iconStyle_, text, component->GetTypeName());
    UIUtils::SetIconEnabledColor(text, component->IsEnabledEffective());
}
