/*!
 * \file MenuBarUI.h
 *
 * \author vitali
 * \date Februar 2015
 *
 *
 */

#pragma once

// Menu selected.
URHO3D_EVENT(E_MENUBAR_ACTION, MenuBarAction)
{
    URHO3D_PARAM(P_ACTION, Action); // stringhash
    URHO3D_PARAM(P_UINAME, UIName);	// string
}

// Use dirty masks
class MenuBarUI : public BorderImage
{
    URHO3D_OBJECT(MenuBarUI, BorderImage);

public:
	// Constructor
    MenuBarUI(Context* context);
	// Destructor
    virtual ~MenuBarUI();
	// Register for Urho3D::Factory
	static void RegisterObject(Context* context);
    
	// Create menu
	static MenuBarUI* Create(UIElement* context, const String& idname, int width = 0, int height = 21, XMLFile* defaultstyle = NULL);

	// Create menu
    Menu* CreateMenu(const String& title);
	// Create menu item
	Menu* CreateMenuItem(const String& menuTitle, const String& title,const StringHash& action = StringHash::ZERO, int accelKey = 0, int accelQual = 0, bool addToQuickMenu = true, String quickMenuText = "");
	
	// Create pupup menu
	Window* CreatePopupMenu(Menu* menu);
	// Create pupup menu item
    Menu* CreatePopupMenuItem(Window* window, const String& title, const StringHash& action = StringHash::ZERO, int accelKey = 0, int accelQual = 0, bool addToQuickMenu = true, String quickMenuText = "");

protected:
	// Finalized pupup menu
    void FinalizedPopupMenu(Window* popup);
	// Create key text
    Text* CreateAccelKeyText(int accelKey, int accelQual);
	// Handle menu select delegate
    void HandleMenuSelected(StringHash eventType, VariantMap& eventData);
};
