#include "EngineIDEStd.h"

#include "../Picker/ResourcePicker.h"
#include "../UIGlobals.h"

#include "AttributeVariableEvents.h"
#include "AttributeVariable.h"
#include "AttributeContainer.h"


class ResourcePicker;

using namespace Urho3D;

AttributeContainer::AttributeContainer(Context* context) : UIElement(context)
{
	m_pEditorResourcePicker = NULL;
	m_pSerializable = NULL;

	m_AttrNameWidth = 150;
	m_AttrHeight = 19;
	m_bIsShowNonEditableAttribute = false;

	SetName("AttributeContainer");
	SetEnabled(true);

	SetLayout(LM_VERTICAL, 4);

	m_pTitleText = CreateChild<Text>("AI_TitleText");
	m_pTitleText->SetInternal(true);

	m_pIconsPanel = m_pTitleText->CreateChild<UIElement>("AI_IconsPanel");
	m_pIconsPanel->SetInternal(true);
	m_pIconsPanel->SetLayout(LM_HORIZONTAL, 2);
	m_pIconsPanel->SetHorizontalAlignment(HA_RIGHT);

	m_pResetToDefault = m_pIconsPanel->CreateChild<Button>("AI_ResetToDefault");
	m_pResetToDefault->SetInternal(true);
	m_pResetToDefault->SetLayout(LM_HORIZONTAL);
	//resetToDefault_->SetHorizontalAlignment(HA_RIGHT);

	BorderImage* img = m_pResetToDefault->CreateChild<BorderImage>();
	img->SetInternal(true);

	m_pAttributeList = CreateChild<ListView>("AI_AttributeList");
	m_pAttributeList->SetInternal(true);
}

AttributeContainer::~AttributeContainer()
{
	DestroyAllDelegates();
}

void AttributeContainer::RegisterObject(Context* context)
{
    context->RegisterFactory<AttributeContainer>();
	// COPY_BASE_ATTRIBUTES(UIElement);
    URHO3D_ATTRIBUTE("Show Non Editable", bool, m_bIsShowNonEditableAttribute, false, AM_FILE);
    URHO3D_ATTRIBUTE("Attr Name Width", int, m_AttrNameWidth, 150, AM_FILE);
    URHO3D_ATTRIBUTE("Attr Height", int, m_AttrHeight, 19, AM_FILE);
}

void AttributeContainer::OnResize()
{
	if (m_pIconsPanel && m_pAttributeList)
    {
        //	iconsPanel_->SetWidth(GetWidth());
    }
}

void AttributeContainer::SetSerializableAttributes(Serializable* serializable, bool createNew)
{
    m_pSerializable = serializable;

	if (m_SerializableType != serializable->GetType() || createNew)
    {
        m_pEditorResourcePicker = GetSubsystem<ResourcePickerManager>();
		m_pAttributeList->RemoveAllItems();
        m_Attributes.Clear();
		m_SerializableType = serializable->GetType();
        CreateSerializableAttributes(serializable);
    }
	else if (m_SerializableType == serializable->GetType())
    {
        UpdateSerializableAttributes(serializable);
    }
}

void AttributeContainer::CreateSerializableAttributes(Serializable* serializable)
{
	m_Attributes.Resize(serializable->GetNumAttributes());

    for (unsigned int i = 0; i < serializable->GetNumAttributes(); ++i)
    {
        AttributeInfo info = serializable->GetAttributes()->At(i);

		if (!m_bIsShowNonEditableAttribute && ((info.mode_ & AM_NOEDIT) != 0))
		{
			continue;
		}

        // Use the default value (could be instance's default value) of the first serializable as the default for all
        info.defaultValue_ = serializable->GetAttributeDefault(i);

        CreateAttribute(serializable, info, i, 0);
    }
}

void AttributeContainer::UpdateSerializableAttributes(Serializable* serializable)
{
    for (unsigned int i = 0; i < m_Attributes.Size(); i++)
    {
        UpdateVariantMap(serializable, i);

        Vector<BasicAttributeUI*>& attrVector = m_Attributes[i];
		for (unsigned int j = 0; j < attrVector.Size(); j++)
        {
            BasicAttributeUI* attr = attrVector[j];
            attr->UpdateVar(serializable);
        }
    }
}

UIElement* AttributeContainer::CreateAttribute(Serializable* serializable, const AttributeInfo& info, unsigned int index, unsigned int subIndex, bool suppressedSeparatedLabel)
{
    UIElement* parent = NULL;

    VariantType type = info.type_;

	if (type == VAR_STRING || type == VAR_BUFFER)
    {
        StringAttributeUI* attr = StringAttributeUI::Create(serializable, info.name_, index,GetDefaultStyle());
		m_pAttributeList->AddItem(attr);
        //	attr->SetStyle("StringAttributeUI");
        attr->SetSubIndex(subIndex);
        //	attr->SetDragDropMode(DD_TARGET);
        parent = attr;

        // Do not subscribe to continuous edits of certain attributes (script class names) to prevent unnecessary errors getting printed
		if (m_NoTextChangedAttrs.Find(info.name_) == m_NoTextChangedAttrs.End())
		{
			attr->GetVarValueUI()->UnsubscribeFromEvent(E_TEXTCHANGED);
		}

		m_Attributes[index].Insert(subIndex, attr);

        SubscribeToEvent(attr, AEE_STRINGVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditStringAttributeDelegate));
	}
	else if (type == VAR_BOOL)
	{
		bool isUIElement = dynamic_cast<UIElement*>(serializable) != NULL;
		BoolAttributeUI* attr = BoolAttributeUI::Create(serializable, info.name_, index, GetDefaultStyle());

		parent = attr;
		attr->SetSubIndex(subIndex);

		if (info.name_ == (isUIElement ? "Is Visible" : "Is Enabled"))
		{
			attr->GetVarNameUI()->SetVisible(false);
			m_pIconsPanel->AddChild(attr);
			SubscribeToEvent(attr, AEE_BOOLVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditEnabledAttributeDelegate));
		}
		else
		{
			m_pAttributeList->AddItem(attr);
			SubscribeToEvent(attr, AEE_BOOLVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditBoolAttributeDelegate));
		}

		//attr->SetStyle("BoolAttributeUI");
		m_Attributes[index].Insert(subIndex, attr);
	}
    else if ((type >= VAR_FLOAT && type <= VAR_VECTOR4) || type == VAR_QUATERNION || type == VAR_COLOR || type == VAR_INTVECTOR2 || type == VAR_INTRECT)
    {
        NumberAttributeUI* attr = NumberAttributeUI::Create(serializable, info.name_, index, type, GetDefaultStyle());

		m_pAttributeList->AddItem(attr);
        //attr->SetStyle("BasicAttributeUI");

        for (int i = 0; i < attr->GetNumCoords(); ++i)
        {
            attr->GetVarValueUI()[i]->SetStyle("EditorAttributeEdit");
        }
        attr->SetSubIndex(subIndex);

        parent = attr;
		m_Attributes[index].Insert(subIndex, attr);

		SubscribeToEvent(attr, AEE_NUMBERVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditNumberAttributeDelegate));
    }
    else if (type == VAR_INT)
    {
        Vector<String> enumNames;

        // Get enums names
        if (info.enumNames_ != NULL)
        {
            const char** enumPtr = info.enumNames_;
            while (*enumPtr)
            {
				enumNames.Push(String(*enumPtr));
                ++enumPtr;
            }
        }

        // Check for enums
		if (enumNames.Empty())
        {
            // No enums, create a numeric editor
            NumberAttributeUI* attr = NumberAttributeUI::Create(serializable, info.name_, index, type, GetDefaultStyle());
			m_pAttributeList->AddItem(attr);
            attr->SetStyle("BasicAttributeUI");

			for (unsigned int i = 0; i < attr->GetNumCoords(); ++i)
            {
                attr->GetVarValueUI()[i]->SetStyle("EditorAttributeEdit");
            }

			attr->SetSubIndex(subIndex);


            // // If the attribute is a node ID, make it a drag/drop target
            // if (info.name_.Contains("NodeID", false) || info.name_.Contains("Node ID", false) || (info.mode_ & AM_NODEID) != 0)
            //	attrEdit->SetDragDropMode(DD_TARGET);

            parent = attr;
			m_Attributes[index].Insert(subIndex, attr);

            SubscribeToEvent(attr, AEE_NUMBERVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditNumberAttributeDelegate));
        }
        else
        {
			EnumAttributeUI* attr = EnumAttributeUI::Create(serializable, info.name_, index, enumNames, m_pAttributeList->GetDefaultStyle());
			m_pAttributeList->AddItem(attr);
            attr->SetSubIndex(subIndex);

            parent = attr;

            m_Attributes[index].Insert(subIndex, attr);

			SubscribeToEvent(attr, AEE_ENUMVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditEnumAttributeDelegate));
		}
    }
    else if (type == VAR_RESOURCEREF)
    {
		if (!m_pEditorResourcePicker)
		{
			return NULL;
		}

        StringHash resourceType;

        // Get the real attribute info from the serializable for the correct resource type
        AttributeInfo attrInfo = serializable->GetAttributes()->At(index);
		if (attrInfo.type_ == VAR_RESOURCEREF)
		{
			resourceType = serializable->GetAttribute(index).GetResourceRef().type_;
		}
		else if (attrInfo.type_ == VAR_RESOURCEREFLIST)
		{
			resourceType = serializable->GetAttribute(index).GetResourceRefList().type_;
		}
		else if (attrInfo.type_ == VAR_VARIANTVECTOR)
		{
			resourceType = serializable->GetAttribute(index).GetVariantVector()[subIndex].GetResourceRef().type_;
		}

		ResourcePicker* picker = m_pEditorResourcePicker->GetResourcePicker(resourceType);

		if (!picker)
		{
			return NULL;
		}

        ResourceRefAttributeUI* attr = ResourceRefAttributeUI::Create(serializable, info.name_, attrInfo.type_,
                                       resourceType, index, subIndex, m_pAttributeList->GetDefaultStyle(), picker->actions);

		if (!attr)
		{
			return NULL;
		}

		m_pAttributeList->AddItem(attr);

		parent = attr;
		m_Attributes[index].Insert(subIndex, attr);

        SubscribeToEvent(attr, AEE_RESREFVARCHANGED, URHO3D_HANDLER(AttributeContainer, EditResRefAttributeDelegate));
    }
    else if (type == VAR_RESOURCEREFLIST)
    {
        unsigned int numRefs = serializable->GetAttribute(index).GetResourceRefList().names_.Size();

        // Straightly speaking the individual resource reference in the list is not an attribute of the serializable
        // However, the AttributeInfo structure is used here to reduce the number of parameters being passed in the function
        AttributeInfo refInfo;
        refInfo.name_ = info.name_;
        refInfo.type_ = VAR_RESOURCEREF;
		for (unsigned int i = 0; i < numRefs; ++i)
		{
			CreateAttribute(serializable, refInfo, index, i, i > 0);
		}
    }
    else if (type == VAR_VARIANTVECTOR)
    {
		if (m_pEditorResourcePicker)
        {
			VectorStruct* vectorStruct = m_pEditorResourcePicker->GetVectorStruct(serializable, index);
			if (vectorStruct == NULL)
			{
				return NULL;
			}

            unsigned int nameIndex = 0;

            Vector<Variant> vector = serializable->GetAttribute(index).GetVariantVector();
            for (unsigned int i = 0; i < vector.Size(); ++i)
            {
                // The individual variant in the vector is not an attribute of the serializable, the structure is reused for convenience
                AttributeInfo vectorInfo;
                vectorInfo.name_ = vectorStruct->variableNames[nameIndex];
                vectorInfo.type_ = vector[i].GetType();
                CreateAttribute(serializable, vectorInfo, index, i);
                ++nameIndex;
				if (nameIndex >= vectorStruct->variableNames.Size())
				{
					nameIndex = vectorStruct->restartIndex;
				}
            }
        }
    }
    else if (type == VAR_VARIANTMAP)
    {
        Vector<BasicAttributeUI*>& varMap = m_Attributes[index];

		for (unsigned int j = 0; j < varMap.Size(); ++j)
		{
			varMap[j]->Remove();
		}

        varMap.Clear();

        VariantMap map = serializable->GetAttribute(index).GetVariantMap();
        Vector<StringHash> keys = map.Keys();
        for (unsigned int i = 0; i < keys.Size(); ++i)
        {
            String varName = GetVariableName(serializable, keys[i]);
            Variant value = map[keys[i]];

            // The individual variant in the map is not an attribute of the serializable, the structure is reused for convenience
            AttributeInfo mapInfo;
            mapInfo.name_ = varName + " (Var)";
            mapInfo.type_ = value.GetType();
            parent = CreateAttribute(serializable, mapInfo, index, i);

            // Add the variant key to the parent. We may fail to add the editor in case it is unsupported
            if (parent != NULL)
            {
                parent->SetVar("Key", keys[i].Value());
                // If variable name is not registered (i.e. it is an editor->IsInternal() variable) then hide it
				if (varName.Empty())
				{
					parent->SetVisible(false);
				}
            }
        }
    }
    return parent;
}

void AttributeContainer::UpdateAttribute(Serializable* serializable, const AttributeInfo& info, unsigned int index, unsigned int subIndex, bool suppressedSeparatedLabel /*= false*/)
{
    BasicAttributeUI* attr = m_Attributes[index][subIndex];
    if (attr)
    {
        attr->UpdateVar(serializable);
    }
}

void AttributeContainer::SetIcon(XMLFile* iconStyle, const String& iconType)
{
	if (!iconStyle || iconType.Empty())
	{
		return;
	}

    // Check if the icon has been created before
    BorderImage* icon = (BorderImage*)m_pTitleText->GetChild(String("Icon"));

    // If iconType is empty, it is a request to remove the existing icon
    if (iconType.Empty())
    {
        // Remove the icon if it exists
		if (icon != NULL)
		{
			icon->Remove();
		}

        // Revert back the indent but only if it is indented by this function
		// if (titleText_->GetVar(INDENT_MODIFIED_BY_ICON_VAR).GetBool()) -edit
        //      titleText_->SetIndent(0);-edit
        return;
    }

    // The UI element must itself has been indented to reserve the space for the icon
	if (m_pTitleText->GetIndent() == 0)
    {
		m_pTitleText->SetIndent(1);
        // m_pTitleText->SetVar(INDENT_MODIFIED_BY_ICON_VAR, true);-edit
    }

    // If no icon yet then create one with the correct indent and size in respect to the UI element
    if (icon == NULL)
    {
        // The icon is placed at one indent level less than the UI element
        icon = new BorderImage(context_);
        icon->SetName("Icon");
		icon->SetIndent(m_pTitleText->GetIndent() - 1);
		icon->SetFixedSize(m_pTitleText->GetIndentWidth() - 2, 14);
		m_pTitleText->InsertChild(0, icon);   // Ensure icon is added as the first child
    }

    // Set the icon type
	if (!icon->SetStyle(iconType, iconStyle))
	{
		icon->SetStyle("Unknown", iconStyle);    // If fails then use an 'unknown' icon type
	}

	icon->SetColor(Color(1, 1, 1, 1)); // Reset to enabled color
}

void AttributeContainer::InitializeAllDelegates()
{

}

void AttributeContainer::DestroyAllDelegates()
{
	UnsubscribeFromAllEvents();
}

void AttributeContainer::EditStringAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    StringAttributeUI* attr = (StringAttributeUI*)eventData[StringVarChanged::P_ATTEDIT].GetPtr();

	if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
    }
}

void AttributeContainer::EditBoolAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    BoolAttributeUI* attr = (BoolAttributeUI*)eventData[BoolVarChanged::P_ATTEDIT].GetPtr();

if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
    }
}

void AttributeContainer::EditEnumAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    EnumAttributeUI* attr = (EnumAttributeUI*)eventData[EnumVarChanged::P_ATTEDIT].GetPtr();
	if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
    }
}

void AttributeContainer::EditNumberAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    NumberAttributeUI* attr = (NumberAttributeUI*)eventData[NumberVarChanged::P_ATTEDIT].GetPtr();
	if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
    }
}

void AttributeContainer::EditEnabledAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    BoolAttributeUI* attr = (BoolAttributeUI*)eventData[BoolVarChanged::P_ATTEDIT].GetPtr();
	if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
        BorderImage* icon = (BorderImage*)m_pTitleText->GetChild(String("Icon"));
        if (icon)
        {
			if (attr->GetVarValue())
			{
				icon->SetColor(Color(1, 1, 1, 1)); // Reset to enabled color
			}
			else
			{
				icon->SetColor(Color(1, 0, 0, 1)); // disabled color
			}
        }
    }
}

void AttributeContainer::EditResRefAttributeDelegate(StringHash eventType, VariantMap& eventData)
{
    ResourceRefAttributeUI* attr = (ResourceRefAttributeUI*)eventData[ResourceRefVarChanged::P_ATTEDIT].GetPtr();
	if (attr && m_pSerializable)
    {
		m_pSerializable->SetAttribute(attr->GetIndex(), attr->GetVariant());
		SetSerializableAttributes(m_pSerializable, true);
    }
}

Urho3D::String AttributeContainer::GetVariableName(Serializable* serializable, StringHash hash)
{
    // First try to get it from scene
    Node* node = dynamic_cast<Node*>(serializable);

    if (node)
    {
        String name = node->GetScene()->GetVarName(hash);
        return name;
    }

    // Then from the UIElement variable names
    // if (name.Empty() && uiElementVarNames_.Contains(hash))
    //		name = uiElementVarNames[hash].ToString();

    return String::EMPTY;   // Since this is a reverse mapping, it does not really matter from which side the name is retrieved back
}
void AttributeContainer::UpdateVariantMap(Serializable* serializable, unsigned int index)
{
    AttributeInfo info = serializable->GetAttributes()->At(index);

	if (info.type_ == VAR_VARIANTMAP)
	{
		VariantMap map = serializable->GetAttribute(index).GetVariantMap();
		Vector<StringHash> keys = map.Keys();

		Vector< BasicAttributeUI* >& varMap = m_Attributes[index];

		for (unsigned int j = 0; j < varMap.Size(); ++j)
		{
			varMap[j]->Remove();
		}

        varMap.Clear();

        for (unsigned int j = 0; j < keys.Size(); ++j)
        {
            String varName = GetVariableName(serializable, keys[j]);
            Variant value = map[keys[j]];

            // The individual variant in the map is not an attribute of the serializable, the structure is reused for convenience
            AttributeInfo mapInfo;
            mapInfo.name_ = varName + " (Var)";
            mapInfo.type_ = value.GetType();
            UIElement* parent = CreateAttribute(serializable, mapInfo, index, j);

            // Add the variant key to the parent. We may fail to add the editor in case it is unsupported
            if (parent != NULL)
            {
                parent->SetVar("Key", keys[j].Value());
                // If variable name is not registered (i.e. it is an editor->IsInternal() variable) then hide it
				if (varName.Empty())
				{
					parent->SetVisible(false);
				}
            }
        }
    }
}

void AttributeContainer::UpdateVariantMap(Serializable* serializable)
{
	if (!serializable)
	{
		return;
	}

    for (unsigned int i = 0; i < serializable->GetNumAttributes(); ++i)
    {
        AttributeInfo info = serializable->GetAttributes()->At(i);

        if (info.type_ == VAR_VARIANTMAP)
        {
            UpdateVariantMap(serializable, i);
        }
    }
}



