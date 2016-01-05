#include "EngineStd.h"
#include "UserInterface/Urho3D/Utilities.h"
#include "ModelessWindow.h"

ModelessWindow::ModelessWindow(Context* context, String text) : BaseUI(context)
{
	m_PosX = m_PosY = 0;
	m_Width = 250;
	m_Height = 100;
	m_Text = text;

	m_pWindow = NULL;
	m_pClosebutton = NULL;

	m_bIsCreated = false;
}

ModelessWindow::~ModelessWindow()
{
	if (m_pWindow)
	{
		g_pApp->GetSubsystem<UI>()->GetRoot()->RemoveChild(m_pWindow);
		
	}
}

void ModelessWindow::VOnUpdate(float timeStep)
{

}

void ModelessWindow::VSetZOrder(int const zOrder)
{
	return;
}

int ModelessWindow::VGetZOrder() const
{
	return 1;
}

void ModelessWindow::VOnShutdown()
{
	return;
}

bool ModelessWindow::VOnLostDevice()
{
	return true;
}

bool ModelessWindow::VOnRestore()
{
	if (!m_bIsCreated)
	{
		m_pWindow = CreateCustomWindow(context_, "ModelessWindow", m_Width, m_Height, IntVector2::ZERO);
		CreateCustomText(m_pWindow, m_Text);
		m_pClosebutton = CreateCloseButton(m_pWindow, "ModelessWindow_CloseButton");
		m_bIsCreated = true;
	}
	return true;
}

bool ModelessWindow::VOnMsgProc(AppMsg message)
{
	return true;
}
