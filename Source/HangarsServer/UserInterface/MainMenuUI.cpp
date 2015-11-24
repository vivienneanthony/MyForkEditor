#include "HangarsServerStd.h"

#include "EngineStd/UserInterface/UserInterface.h"
#include "EngineStd/UserInterface/Urho3D/Utilities.h"

#include "EngineStd/GameLogic/BaseGameLogic.h"
#include "EngineStd/GameLogic/LevelManager/LevelManager.h"
#include "EngineStd/Resources/ResHandle.h"
#include "EngineStd/Resources/ResourceCache.h"

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
		CreateServerWindow();
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

}

bool MainMenuUI::VOnMsgProc(AppMsg message)
{
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

void MainMenuUI::CreateServerWindow()
{
	m_pWindow = CreateCustomWindow(context_, "ServerWindow", IntVector2(0, -50));
	
	Text* textName = new Text(context_);
	textName->SetText("Enter port: ");

	textName->SetTextAlignment(HorizontalAlignment::HA_CENTER);
	textName->SetStyleAuto();

	m_pWindow->AddChild(textName);
	m_pWindow->SetFixedHeight(300);
	m_pWindow->SetPosition(0, -100);

	// Create a LoginEdit
	m_pPortEdit = new LineEdit(context_);
	m_pPortEdit->SetName("LoginEdit");
	m_pPortEdit->SetMinHeight(24);
	m_pPortEdit->SetStyleAuto();
	m_pPortEdit->SetFixedWidth(350);
	m_pPortEdit->SetFixedHeight(20);
	m_pPortEdit->SetAlignment(HorizontalAlignment::HA_CENTER, VA_TOP);
	m_pWindow->AddChild(m_pPortEdit);

	Button* enterButton = CreateCustomButton(m_pWindow, "Enter", "StartServer");
	enterButton->SetFixedWidth(100);
	SubscribeToEvent(enterButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleStartServerDelegate));
}

void MainMenuUI::HandleStartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	int port = ToInt(m_pPortEdit->GetText());
	g_pApp->GetGameOptions().m_ListenPort = port;
	g_pApp->GetGameOptions().m_GameHost = "localhost";
	
	//SendEvent(String("Request_Start_Game"));
}


