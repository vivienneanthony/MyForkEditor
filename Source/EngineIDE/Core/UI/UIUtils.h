//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#pragma once

namespace UIUtils
{
//////////////////////////////////////////////////////////////////////////
///  Editor UI Creation Function
//////////////////////////////////////////////////////////////////////////
extern void		CreateDir(const Object* obj, const String& pathName, String baseDir = String::EMPTY);
extern Menu*		CreateMenu(Context* context, XMLFile* uiStyle, const String& title);
//extern//Menu*		CreateMenuItem(Context* context, XMLFile* uiStyle, const String& title, EventHandler* handler = NULL, int accelKey = 0, int accelQual = 0, bool addToQuickMenu = true, String quickMenuText = "");
extern Menu*		CreateMenuItem(Context* context, XMLFile* uiStyle, const String& title, EventHandler* handler = NULL, int accelKey = 0, int accelQual = 0, bool addToQuickMenu = true, String quickMenuText = "");
extern Text* 		CreateAccelKeyText(Context* context, XMLFile* uiStyle, int accelKey, int accelQual);
extern Window*		CreatePopup(Context* context, XMLFile* uiStyle, Menu* baseMenu);
extern void		FinalizedPopupMenu(Window* popup);
extern void		CreateChildDivider(UIElement* parent);
extern UIElement*	CreateGroup(Context* context, XMLFile* uiStyle, const String& title, LayoutMode layoutMode);
extern CheckBox*	CreateToolBarToggle(Context* context, XMLFile* uiStyle, XMLFile* iconStyle_, const String& title);
extern void		CreateToolBarIcon(Context* context, XMLFile* iconStyle_, UIElement* element);
extern UIElement*	CreateToolTip(UIElement* parent, const String& title, const IntVector2& offset);
extern void		FinalizeGroupHorizontal(UIElement* group, const String& baseStyle);
extern Button*		CreateSmallToolBarButton(Context* context, XMLFile* uiStyle, XMLFile* iconStyle_, const String& title, String toolTipTitle = "");
extern void		CreateSmallToolBarIcon(Context* context, XMLFile* iconStyle_, UIElement* element);
extern UIElement*	CreateSmallToolBarSpacer(unsigned int width);

//////////////////////////////////////////////////////////////////////////
///  Hierarchy Window Function
//////////////////////////////////////////////////////////////////////////
extern String	GetUIElementTitle(UIElement* element);
extern String	GetComponentTitle(Component* component);
extern String	GetNodeTitle(Node* node);
extern Variant		GetUIElementID(UIElement* element);
extern unsigned int	GetID(Serializable* serializable, int itemType);
extern int GetType(Serializable* serializable);
extern bool	MatchID(UIElement* element, const Variant& id, int itemType);
extern void	SetIconEnabledColor(UIElement* element, bool enabled, bool partial = false);

extern void	IconizeUIElement(Context* context, XMLFile* iconStyle_, UIElement* element, const String& iconType);
extern Vector<Serializable*> ToSerializableArray(Vector<Node*> nodes);

//////////////////////////////////////////////////////////////////////////
///  Attribute Window Function
//////////////////////////////////////////////////////////////////////////
extern UIElement*	CreateAttributeEditorParent(ListView* list, const String& name, unsigned int index, unsigned int subIndex);
extern UIElement*	CreateAttributeEditorParentAsListChild(ListView* list, const String& name, unsigned int index, unsigned int subIndex);
extern LineEdit*	CreateAttributeLineEdit(UIElement* parent, Vector<Serializable*>& serializables, unsigned int index, unsigned int subIndex);
extern void		SetAttributeEditorID(UIElement* attrEdit, Vector<Serializable*>& serializables);
extern void		SetAttributeEditorID(UIElement* attrEdit, Serializable* serializables);
extern UIElement*	CreateBoolAttributeEditor(ListView* list, Vector<Serializable*>& serializables, const AttributeInfo& info, unsigned int index, unsigned int subIndex);
extern UIElement*	CreateStringAttributeEditor(ListView* list, Vector<Serializable*>& serializables, const AttributeInfo& info, unsigned int index, unsigned int subIndex);
extern UIElement*	CreateNumAttributeEditor(ListView* list, Vector<Serializable*>& serializables, const AttributeInfo& info, unsigned int index, unsigned int subIndex);
extern UIElement*	CreateIntAttributeEditor(ListView* list, Vector<Serializable*>& serializables, const AttributeInfo& info, unsigned int index, unsigned int subIndex);

/// scene Editor, menu bar actions
extern String ExtractFileName(VariantMap& eventData, bool forSave = false);

}
