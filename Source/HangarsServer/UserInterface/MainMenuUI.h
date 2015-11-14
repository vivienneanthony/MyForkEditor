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
	void CreateServerWindow();

	// UI Login delegates
	void HandleStartServerDelegate(StringHash eventType, VariantMap& eventData);
	void HandleStopServerDelegate(StringHash eventType, VariantMap& eventData);

protected:
	bool m_bIsInitialized;
	
	Window* m_pWindow;
	LineEdit* m_pPortEdit;
};

#endif //MAIN_MENU_UI_H
