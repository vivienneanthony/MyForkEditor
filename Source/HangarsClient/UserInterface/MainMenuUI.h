#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "AlphaEngine/GameLogic/LevelManager/LevelManager.h"

#include "AlphaEngine/UserInterface/UserInterface.h"

class MainMenuView;
class ModelessWindow;

class MainMenuUI : public BaseUI
{
	URHO3D_OBJECT(MainMenuUI, BaseUI);

public:
	MainMenuUI(Context* context, MainMenuView* menu);
	virtual ~MainMenuUI();

	virtual bool VOnRestore();
	virtual bool VOnLostDevice();
	virtual void VOnUpdate(float timeStep);
	virtual void VOnShutdown();

	virtual bool VOnMsgProc(AppMsg message);

	virtual int VGetZOrder() const;
	virtual void VSetZOrder(int const zOrder);

	virtual bool VIsVisible() const;
	virtual void VSetVisible(bool visible);

	// Helper functions
	void CreateLoginWindow();

	// UI Login delegates
	void HandleEnterDelegate(StringHash eventType, VariantMap& eventData);
	void HandlePlayerLoginResult(StringHash eventType, VariantMap& eventData);

	// Modeless window delegate
	void HandleModelessWindow(StringHash eventType, VariantMap& eventData);

	// UI Otions delegates
	void HandleCloseDelegate(StringHash eventType, VariantMap& eventData);
	void HandleOptionCloseDelegate(StringHash eventType, VariantMap& eventData);
	void HandleVideoOptionCloseDelegate(StringHash eventType, VariantMap& eventData);

	// Audio sliders
	void HandleMasterSliderDelegate(StringHash eventType, VariantMap& eventData);
	void HandleSoundSliderDelegate(StringHash eventType, VariantMap& eventData);
	void HandleMusicSliderDelegate(StringHash eventType, VariantMap& eventData);
	void HandleVoiceSliderDelegate(StringHash eventType, VariantMap& eventData);

	// Video settings
	void HandleResListSelectedDelegate(StringHash eventType, VariantMap& eventData);
	void HandleWindowCheckBoxDelegate(StringHash eventType, VariantMap& eventData);
	void HandleVSyncCheckBoxDelegate(StringHash eventType, VariantMap& eventData);
	void HandleMultisampleListDelegate(StringHash eventType, VariantMap& eventData);
	void HandleTripleBufferCheckBoxDelegate(StringHash eventType, VariantMap& eventData);

protected:
	bool m_bIsInitialized;
	MainMenuView* m_pMainMenuView;

	SharedPtr<Scene> m_pMainMenuScene;

	Window* m_pWindow;

	Node* m_pMusicNode;
	Node* m_pSelectSoundNode;

	LineEdit* m_pLoginEdit;
	LineEdit* m_pPasswordEdit;

	SharedPtr<ModelessWindow> m_ModelessWindow;
};

#endif //MAIN_MENU_UI_H
