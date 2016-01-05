#pragma once

#include "UserInterface/UserInterface.h"

class ModelessWindow : public BaseUI
{
	URHO3D_OBJECT(ModelessWindow, BaseUI);

public:
	ModelessWindow(Context* context, String text);
	virtual ~ModelessWindow();

	// Default virtuals must be defined
	virtual bool VOnRestore();
	virtual bool VOnLostDevice();
	virtual void VOnUpdate(float timeStep);
	virtual void VOnShutdown();

	virtual bool VOnMsgProc(AppMsg message);

	virtual int VGetZOrder() const;
	virtual void VSetZOrder(int const zOrder);

	Window* GetWindow() { return m_pWindow; }
	Button* GetCloseButton() { return m_pClosebutton; }
private:
	Window* m_pWindow; 
	Button* m_pClosebutton;
	String m_Text;

	bool m_bIsCreated;
};