/*!
 * \file ToolBarUI.h
 *
 * \author vitali
 * \date Februar 2015
 *
 *
 */

#pragma once

#include "../UI/ToolTip.h"

/// \todo use dirty masks
class ToolBarUI : public BorderImage
{
    URHO3D_OBJECT(ToolBarUI, BorderImage);
public:
	// Constructor
	ToolBarUI(Context* context);
	// Destructor
	virtual ~ToolBarUI();
	// Register for Urho3D::Factory
	static void RegisterObject(Context* context);
	// Create tool bar
    static ToolBarUI* Create(UIElement* context, const String& idname, XMLFile* iconStyle, const String& baseStyle = "ToolBarToggle", int width = 0, int height = 41, XMLFile* defaultstyle = NULL);

	// Create group
    UIElement* CreateGroup( const String& name, LayoutMode layoutmode);
	// Create checkboxes within group 
	CheckBox* CreateToolBarToggle(const String& groupname, const String& title);
	// Create checkbox
	CheckBox* CreateToolBarToggle(const String& title);
	// Create icon
	UIElement* CreateToolBarIcon(UIElement* element);
	// Create tool tip
	UIElement* CreateToolTip(UIElement* parent, const String& title, const IntVector2& offset);
	// Create spacer
	UIElement* CreateToolBarSpacer(int width);
    
	// Getters/Setters
	// Set icon style
	void SetIconStyle(XMLFile* iconStyle) { m_pIconStyle = iconStyle; }
	// Set base style
	void SetBaseStyle(const String& baseStyle) { m_BaseStyle = baseStyle; }

protected:
	// Finalize creation
	void FinalizeGroupHorizontal(UIElement* group, const String& baseStyle);

protected:
    // Icon style
	SharedPtr<XMLFile> m_pIconStyle;
    // Horizontal scroll bar.
    SharedPtr<ScrollBar> m_pHorizontalScrollBar;
    // Base style
	String m_BaseStyle;
};
