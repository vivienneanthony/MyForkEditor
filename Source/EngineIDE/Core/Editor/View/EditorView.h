#pragma once


using namespace Urho3D;

// User inteface
#include "../../UI/Tab/TabWindow.h"
#include "../../UI/Menu/ToolBarUI.h"
#include "../../UI/Menu/MenuBarUI.h"
#include "../../UI/Menu/MiniToolBarUI.h"

#include "../../Utils/Macros.h"

class EditorView : public Object
{
    URHO3D_OBJECT(EditorView, Object);
public:
    // Constructor
    EditorView(Context* context);
    // Destructor
    virtual ~EditorView();

	// Register for Urho3D::Factory
	static void RegisterObject(Context* context);

	// Update layout
    void UpdateLayout();

	// Call first. Initialize.
	bool Initialize(Editor* editor, EditorData* data);

	// Getters/Setters
	// Get menu bar
	MenuBarUI* GetMenuBar() { return m_pMenubar; }
	// Get tool bar
	ToolBarUI* GetToolBar() { return m_pToolbar; }
	// Get mini tool bar
	MiniToolBarUI* GetMiniToolBar() { return m_pMiniToolbar; }
	// Get left frame
	TabWindow* GetLeftFrame() { return m_pLeftFrame; }
	// Get right frame
	TabWindow* GetRightFrame() { return m_pRightFrame; }
	// Get middle frame
	TabWindow* GetMiddleFrame() { return m_pMiddleFrame; }

	// Is tool bar visible
	bool IsToolBarVisible() const { return m_pToolbar->IsVisible(); }
	// Is mini tool bar visible
	bool IsMiniToolBarVisible() const { return m_pMiniToolbar->IsVisible(); }
	// Set mini tool bar visible
	void SetMiniToolBarVisible(bool enable) { m_pMiniToolbar->SetVisible(enable); UpdateLayout(); }
	// Set tool bar visible
	void SetToolBarVisible(bool enable) { m_pToolbar->SetVisible(enable); UpdateLayout(); }
	// Is left frame visible
	bool IsLeftFrameVisible() const { return m_pLeftFrame->IsVisible(); }
	// Set left frame visible
	void SetLeftFrameVisible(bool enable) { m_pLeftFrame->SetVisible(enable); UpdateLayout(); }
	// Is right frame visible
	bool IsRightFrameVisible() const { return m_pRightFrame->IsVisible(); }
	// Set right frame visible
	void SetRightFrameVisible(bool enable) { m_pRightFrame->SetVisible(enable); UpdateLayout(); }

protected:
	// Create UI
	// Create menu
	bool CreateMenu(XMLFile* styleFile, XMLFile* iconstyle);
	// Create tool bar
	bool CreateToolBar(XMLFile* styleFile, XMLFile* iconstyle);
	// Create main frame
	bool CreateMainFrame(XMLFile* styleFile, XMLFile* iconstyle);
	// Create mini tool bar
	bool CreateMiniToolBar(XMLFile* styleFile, XMLFile* iconstyle);
	// Create left frame
	bool CreateLeftFrame(XMLFile* styleFile, XMLFile* iconstyle);
	// Create middle frame
	bool CreateMiddleFrame(XMLFile* styleFile, XMLFile* iconstyle);
	// Create right frame
	bool CreateRightFrame(XMLFile* styleFile, XMLFile* iconstyle);

	// Delegates
	// Initialize all delegates
	void InitializeAllDelegates();
	// Destroy all delegates
	void DestroyAllDelegates();
	// Handle screen updates events
	void HandleScreenUpdateDelegate(StringHash eventType, VariantMap& eventData);
	// Handle resize events
	void HandleResizeEventDelegate(StringHash eventType, VariantMap& eventData);

protected:
    // Cached Subsystems
    ResourceCache* m_pCache;
	// Cached Subsystems
	Graphics* m_pGraphics;;
	// Cached Subsystems
    UI* m_pUI;

	// Root UI
    SharedPtr<UIElement> m_pUIRoot;
	// Menu bar
    SharedPtr<MenuBarUI> m_pMenubar;
	// Tool bar
    SharedPtr<ToolBarUI> m_pToolbar;
	// Mini tool bar
    SharedPtr<MiniToolBarUI> m_pMiniToolbar;
	// Main frame
	SharedPtr<UIElement> m_pMainFrame;
	// Left frame
    SharedPtr<TabWindow> m_pLeftFrame;
	// Middle frame
    SharedPtr<TabWindow> m_pMiddleFrame;
	// Right frame
    SharedPtr<TabWindow> m_pRightFrame;

	// Editor data
    EditorData* m_pEditorData;

	// Height (Window height - (menu + toolbar height))
	int m_MiniToolHeight;

	// Cocos properties
	U_PROPERTY_IMP(int, m_MinLeftFrameWidth, MinLeftFrameWidth);
	//U_PROPERTY_IMP(int, m_MinMiddleFrameWidth, MinMiddleFrameWidth);
	U_PROPERTY_IMP(int, m_MinRightFrameWidth, MinRightFrameWidth);
	U_PROPERTY_IMP(int, m_MaxLeftFrameWidth, MaxLeftFrameWidth);
	U_PROPERTY_IMP(int, m_MaxRightFrameWidth, MaxRightFrameWidth);
};
