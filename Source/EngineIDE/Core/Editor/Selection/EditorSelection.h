#pragma once


class EditorSelection : public Object
{
    URHO3D_OBJECT(EditorSelection,Object);
public:
    EditorSelection(Context* context, Editor* editor = NULL);
    virtual ~EditorSelection();
    static void RegisterObject(Context* context);
    ///  Selection
    void ClearSelection();


    void AddSelectedComponent(Component* comp);
    void AddSelectedNode(Node* node);
    void AddEditComponent(Component* comp);
    void AddEditNode(Node* node);
    void AddSelectedUIElement(UIElement* element);
    void AddEditUIElement(UIElement* element);

    unsigned GetNumSelectedUIElements();
    unsigned GetNumSelectedComponents();
    unsigned GetNumSelectedNodes();
    unsigned GetNumEditComponents();
    unsigned GetNumEditNodes();

      // Added Bluefish fix - 12.12.15
    Vector<WeakPtr<Node>>&		GetSelectedNodes();
    Vector<WeakPtr<Component>>&	GetSelectedComponents();
    Vector<WeakPtr<UIElement>>&	GetSelectedUIElements();
    Vector<WeakPtr<Node>>&		GetEditNodes();
    Vector<WeakPtr<Component>>&	GetEditComponents();
    UIElement*			GetEditUIElement();
    //Vector<UIElement*>	GetEditUIElements();

    // Added Bluefish fix - 12.12.15
    Vector<WeakPtr<UIElement>>	GetEditUIElements();
    Node*				GetEditNode();
    unsigned int		GetNumEditableComponentsPerNode();

    void	SetEditNode(Node* node);
    void	SetEditUIElement(UIElement* element);
    // Added Bluefish fix - 12.12.15
    void	SetSelectedNodes(Vector<WeakPtr<Node>>& nodes);
    void	SetSelectedComponents(Vector<WeakPtr<Component>>& comps);
    void	SetSelectedUIElements(Vector<WeakPtr<UIElement>>& elemets);
    void	SetEditNodes(Vector<WeakPtr<Node>>& nodes);
    void	SetEditComponents(Vector<WeakPtr<Component>>& comps);
    void	SetEditUIElements(Vector<WeakPtr<UIElement>>& elements);
    void	SetNumEditableComponentsPerNode(unsigned int num);

    void			SetGlobalVarNames(const String& name);
    const Variant&	GetGlobalVarNames(StringHash& name);

    void OnHierarchyListSelectionChange(const PODVector<UIElement*>& items, const PODVector<unsigned>& indices);
protected:
    /// Selection
    // Added Bluefish fix - 12.12.15
    Vector<WeakPtr<Node>>		selectedNodes_;
    Vector<WeakPtr<Component>>	selectedComponents_;
    Vector<WeakPtr<UIElement>>	selectedUIElements_;

    UIElement*	editUIElement_;
    Node*		editNode_;
    Editor* editor_;

    // Added Bluefish fix - 12.12.15
    Vector<WeakPtr<Node>>		editNodes_;
    Vector<WeakPtr<Component>>	editComponents_;
    Vector<WeakPtr<UIElement>>	editUIElements_;

    unsigned int numEditableComponentsPerNode_;

    // Node or UIElement hash-to-varname reverse mapping
    VariantMap globalVarNames_;



};

