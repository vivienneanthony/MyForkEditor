#include <HangarsClientStd.h>

#include <EngineStd/UserInterface/UserInterface.h>

#include <EngineStd/GameLogic/BaseGameLogic.h>
#include <EngineStd/GameLogic/LevelManager/LevelManager.h>
#include <EngineStd/Resources/ResHandle.h>
#include <EngineStd/Resources/ResourceCache.h>

#include "MainMenuUI.h"

MainMenuUI::MainMenuUI(Context* context) : BaseUI(context)
{

	m_bIsInitialized = false;
	m_pWindow = nullptr;
}

MainMenuUI::~MainMenuUI()
{


}

void MainMenuUI::VOnUpdate(float timeStep)
{

}

bool MainMenuUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{


		m_bIsInitialized = true;
		return true;
	}


	return false;
}

bool MainMenuUI::VOnLostDevice()
{

	return true;
}

void MainMenuUI::VOnShutdown()
{
	m_pWebViewScene->RemoveChild(m_pMusicNode);
}

bool MainMenuUI::VOnMsgProc(AppMsg message)
{

	if (m_pWindow)
	{
		if (message.uEvent == MOUSE_LBUTTONDOWN)
		{
			m_pWindow->BringToFront();
		}
	}
	return false;
}

int MainMenuUI::VGetZOrder() const
{
	return 1;
}

void MainMenuUI::VSetZOrder(int const zOrder)
{

}

bool MainMenuUI::VIsVisible() const
{

	return true;
}

void MainMenuUI::VSetVisible(bool visible)
{


}


