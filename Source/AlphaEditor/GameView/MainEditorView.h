#ifndef MAIN_MENU_VIEW_H
#define MAIN_MENU_VIEW_H

#include "AlphaEngine/UserInterface/HumanView/HumanView.h"

class Editor;

class MainEditorView : public HumanView
{
	URHO3D_OBJECT(MainEditorView, HumanView)
public:
	MainEditorView(Context* context, Renderer* renderer, bool intro);
	virtual ~MainEditorView();

	// Initialization.
	virtual bool VOnRestore() { return true; }
	virtual void VShutdown() { }

protected:
	// Delegates
	virtual void VInitializeAllDelegates();				// Register all delegates
	virtual void VDestroyAllDelegates();				// Unsubscribe from all events

	// Delegates
	void FinishIntroductionPartDelegate(StringHash eventType, VariantMap& eventData);

	virtual void EditorInstance(void);
    virtual void HandleEditorInstanceClose(StringHash eventType, VariantMap& eventData);

    virtual void CreateConsoleAndDebugHud(void);
    virtual void HandleKeyDown(StringHash eventType, VariantMap& eventData);


public:
	SharedPtr<IScreenElement> m_pMainMenu;

    SharedPtr<Viewport> m_pViewport;

    WeakPtr<UIElement> m_pUIRootElement;
    WeakPtr<DebugHud> m_pDebugHud;
    WeakPtr<Console> m_pConsole;

    Editor * m_pEditor;
};

#endif
