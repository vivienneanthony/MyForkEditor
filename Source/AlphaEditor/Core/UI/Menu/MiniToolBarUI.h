/*!
 * \file MiniToolBarUI.h
 *
 * \author vitali
 * \date Februar 2015
 *
 *
 */

#pragma once

class MiniToolBarUI : public BorderImage
{
    URHO3D_OBJECT(MiniToolBarUI, BorderImage);

public:
	// Constructor
    MiniToolBarUI(Context* context);
	// Destructor
    virtual ~MiniToolBarUI();
	// Register for Urho3D::Factory
    static void RegisterObject(Context* context);
	
	// Create mini tool bar
	static MiniToolBarUI* Create(UIElement* context, const String& idname, XMLFile* iconStyle, int width = 28, int height = 0, XMLFile* defaultstyle = NULL);

	// Create small tool bar button
	UIElement* CreateSmallToolBarButton(const String& title, const String& toolTipTitle = String::EMPTY);
	// Create small tool bar spacer
	UIElement* CreateSmallToolBarSpacer(unsigned int width);

	// Getters/Setters
	// Set icon style
	void SetIconStyle(XMLFile* iconStyle) { m_pIconStyle = iconStyle; }

protected:
	// Create small tool bar icon
    void CreateSmallToolBarIcon( UIElement* element);
	// Create stool tip
    UIElement* CreateToolTip(UIElement* parent, const String& title, const IntVector2& offset);
   
protected:
	SharedPtr<XMLFile> m_pIconStyle;
};
