#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "AlphaEngine/GameLogic/LevelManager/LevelManager.h"

#include "AlphaEngine/UserInterface/UserInterface.h"

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
	void CreateServerWindow();

	// UI Login delegates
	void HandleServerOptionDelegate(StringHash eventType, VariantMap& eventData);
	void HandlePlayersInfoDelegate(StringHash eventType, VariantMap& eventData);
	void HandleServerInfoDelegate(StringHash eventType, VariantMap& eventData);

	void HandleStartServerDelegate(StringHash eventType, VariantMap& eventData);
	void HandleStopServerDelegate(StringHash eventType, VariantMap& eventData);
	void HandleRestartServerDelegate(StringHash eventType, VariantMap& eventData);
	void HandlePauseServerDelegate(StringHash eventType, VariantMap& eventData);

	void HandleServerCreatedDelegate(StringHash eventType, VariantMap& eventData);
	void HandleServerStoppedDelegate(StringHash eventType, VariantMap& eventData);


	void HandleCloseDelegate(StringHash eventType, VariantMap& eventData);

protected:
	bool m_bIsInitialized;
	bool m_bIsCreated;

	Input* m_pInput;

	SharedPtr<UIElement> m_pControlPanel;

	Menu* m_pServerInfoMenu;
	Menu* m_pPlayersInfoMenu;
	Menu* m_pServerOptionMenu;

	Window* m_pPlayersInfoWindow;
	Window* m_pServerInfoWindow;
	Window* m_pServerOptionWindow;

	LineEdit* m_pPortEdit;
};

#endif //MAIN_MENU_UI_H
