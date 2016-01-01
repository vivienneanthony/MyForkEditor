#pragma once

class BasicAttributeUI;
class EditorResourcePicker;
class ResourcePickerManager;

class AttributeContainer : public UIElement
{
    URHO3D_OBJECT(AttributeContainer, UIElement);

public:
    // Construct.
    AttributeContainer(Context* context);
    // Destruct.
    virtual ~AttributeContainer();
    // Register object factory.
    static void RegisterObject(Context* context);

	// Update variant map
	void UpdateVariantMap(Serializable* serializable);
	// Update variant map
	void UpdateVariantMap(Serializable* serializable, unsigned int index);
	
	// React to resize.
    virtual void OnResize();

	// Getter/Setters
	// Get attribute list
	ListView* GetAttributeList() { return m_pAttributeList; }
	// Get serializable
	Serializable* GetSerializable() { return m_pSerializable; }
	// Get ResetToDefault button
	Button* GetResetToDefaultButton() { return m_pResetToDefault; }
	// Set title
	void SetTitle(const String& title) { m_pTitleText->SetText(title); }
	// Set icon
	void SetIcon(XMLFile* iconStyle, const String& iconType);
	// Set no text changed attributes
	void SetNoTextChangedAttrs(const Vector<String>& noTextChangedAttrs) { m_NoTextChangedAttrs = noTextChangedAttrs; }
	// Set serializable attributes
    void SetSerializableAttributes(Serializable* serializable, bool createNew = false);

protected:
	// Create serialiable attributes
    void CreateSerializableAttributes(Serializable* serializable);
	// Update serialiable attributes
	void UpdateSerializableAttributes(Serializable* serializable);

	// Create attribute
    UIElement* CreateAttribute(Serializable* serializable, const AttributeInfo& info, unsigned int index, unsigned int subIndex, bool suppressedSeparatedLabel = false);
    // Update attribute
	void UpdateAttribute(Serializable* serializable, const AttributeInfo& info, unsigned int index, unsigned int subIndex, bool suppressedSeparatedLabel = false);

	// Get Variable name
    String GetVariableName(Serializable* serializable, StringHash hash);

	// Delegates
	// Initialize all delegates
	void InitializeAllDelegates();
	// Destroy all delegates
	void DestroyAllDelegates();
	// Edit enabled attribute
    void EditEnabledAttributeDelegate(StringHash eventType, VariantMap& eventData);
	// Edit string attribute
	void EditStringAttributeDelegate(StringHash eventType, VariantMap& eventData);
	// Edit bool attribute
	void EditBoolAttributeDelegate(StringHash eventType, VariantMap& eventData);
	// Edit enum attribute
	void EditEnumAttributeDelegate(StringHash eventType, VariantMap& eventData);
	// Edit number attribute
	void EditNumberAttributeDelegate(StringHash eventType, VariantMap& eventData);
	// Edit resource reference attribute
	void EditResRefAttributeDelegate(StringHash eventType, VariantMap& eventData);

protected:
    // UI Attributes
	// Title
    SharedPtr<Text>	m_pTitleText;
    // Icons panel
	SharedPtr<UIElement> m_pIconsPanel;
    // Spacer
	SharedPtr<UIElement> m_pSpacer;
    // Reset to default button
	SharedPtr<Button> m_pResetToDefault;
    // Attribute list
	SharedPtr<ListView> m_pAttributeList;
    // Basic attributes
	Vector<Vector<BasicAttributeUI*>> m_Attributes;

    // Other attributes
	// Serializable type
    StringHash m_SerializableType;
	// Serializable
    Serializable* m_pSerializable;
	// Resource picker
    ResourcePickerManager* m_pEditorResourcePicker;

    // Exceptions for string attributes that should not be continuously edited
    Vector<String> m_NoTextChangedAttrs;

    // Serialized Attributes
	// Show non editable attributes
    bool m_bIsShowNonEditableAttribute;
    // Attribute's name width
	int	m_AttrNameWidth;
	// Attribute's height
    int	m_AttrHeight;
};
