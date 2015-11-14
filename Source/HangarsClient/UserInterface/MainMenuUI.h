#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "EngineStd/GameLogic/LevelManager/LevelManager.h"

#include "EngineStd/UserInterface/UserInterface.h"

class MainMenuUI : public BaseUI
{
	URHO3D_OBJECT(MainMenuUI, BaseUI);

public:
	MainMenuUI(Context* context);
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
	//void PlaySelectSound();

	// UI Login delegates
	void HandleEnterDelegate(StringHash eventType, VariantMap& eventData);
	
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
	SharedPtr<Scene> m_pMainMenuScene;

	Window* m_pWindow;

	Node* m_pMusicNode;
	Node* m_pSelectSoundNode;

	LineEdit* m_pLoginEdit;
	LineEdit* m_pPasswordEdit;
};

#endif //MAIN_MENU_UI_H
