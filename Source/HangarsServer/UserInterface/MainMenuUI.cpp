#include "HangarsServerStd.h"

#include "EngineStd/UserInterface/UserInterface.h"
#include "EngineStd/UserInterface/Urho3D/Utilities.h"

#include "EngineStd/EventManager/Server/ServerEvents.h"

#include "EngineStd/GameLogic/BaseGameLogic.h"
#include "EngineStd/GameLogic/LevelManager/LevelManager.h"
#include "EngineStd/Resources/ResHandle.h"
#include "EngineStd/Resources/ResourceCache.h"

#include "MainMenuUI.h"

MainMenuUI::MainMenuUI(Context* context) : BaseUI(context)
{
	m_bIsInitialized = false;
	m_pControlPanel= nullptr;
	m_pInput = GetSubsystem<Input>();
	m_bIsCreated = false;
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
	if(m_pInput->GetKeyPress(Urho3D::KEY_F1))
	{
		CreateServerWindow();
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

void MainMenuUI::CreateServerWindow()
{
	if (!m_bIsCreated)
	{
		UI* ui = GetSubsystem<UI>();
		XMLFile* file = g_pApp->GetConstantResCache()->GetResource<XMLFile>("Server/ControlPanelUI.xml");

		m_pControlPanel = ui->LoadLayout(file);
		ui->GetRoot()->AddChild(m_pControlPanel);

		m_pPlayersInfoWindow = (Window*)m_pControlPanel->GetChild("Window_Server_Players_Info", true);
		m_pServerInfoWindow = (Window*)m_pControlPanel->GetChild("Window_Server_Info", true);
		m_pServerOptionWindow = (Window*)m_pControlPanel->GetChild("Window_Server_Option", true);

		// Set delegates
		m_pPlayersInfoMenu = (Menu*)m_pControlPanel->GetChild("Menu_Server_Players_Info", true);
		SubscribeToEvent(m_pPlayersInfoMenu, E_MENUSELECTED, URHO3D_HANDLER(MainMenuUI, HandlePlayersInfoDelegate));

		m_pServerInfoMenu = (Menu*)m_pControlPanel->GetChild("Menu_Server_Info", true);
		SubscribeToEvent(m_pServerInfoMenu, E_MENUSELECTED, URHO3D_HANDLER(MainMenuUI, HandleServerInfoDelegate));

		m_pServerOptionMenu = (Menu*)m_pControlPanel->GetChild("Menu_Server_Option", true);
		SubscribeToEvent(m_pServerOptionMenu, E_MENUSELECTED, URHO3D_HANDLER(MainMenuUI, HandleServerOptionDelegate));

		m_pPortEdit = (LineEdit*)m_pControlPanel->GetChild("LineEdit_Server_Port", true);

		Button* closeButton = (Button*)m_pControlPanel->GetChild("Close_button", true);
		SubscribeToEvent(closeButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleCloseDelegate));

		Button* startButton = (Button*)m_pControlPanel->GetChild("Button_Server_Start", true);
		Button* restartButton = (Button*)m_pControlPanel->GetChild("Button_Server_Restart", true);
		Button* stopButton = (Button*)m_pControlPanel->GetChild("Button_Server_Stop", true);
		Button* pauseButton = (Button*)m_pControlPanel->GetChild("Button_Server_Pause", true);

		SubscribeToEvent(startButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleStartServerDelegate));
		SubscribeToEvent(restartButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleRestartServerDelegate));
		SubscribeToEvent(stopButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleStopServerDelegate));
		SubscribeToEvent(pauseButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandlePauseServerDelegate));

		SubscribeToEvent(Event_Data_Server_Create_Result::g_EventType, URHO3D_HANDLER(MainMenuUI, HandleServerCreatedDelegate));
		SubscribeToEvent(Event_Data_Server_Stop_Result::g_EventType, URHO3D_HANDLER(MainMenuUI, HandleServerStoppedDelegate));

		m_bIsCreated = true;
	}
	else
	{
		m_pControlPanel->SetVisible(true);
	}
}

void MainMenuUI::HandleStartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	int port = ToInt(m_pPortEdit->GetText());
	g_pApp->GetGameOptions().m_ListenPort = port;
	g_pApp->GetGameOptions().m_GameHost = "unknown";
	
	if (!g_pApp->GetGameLogic()->IsServerCreated())
	{
		SendEvent(Event_Data_Start_Server_Request::g_EventType);
	}
	else
	{
		URHO3D_LOGINFO("Could not create new server. Server has already created.");
	}
}

void MainMenuUI::HandleStopServerDelegate(StringHash eventType, VariantMap& eventData)
{
	if (g_pApp->GetGameLogic()->IsServerCreated())
	{
		SendEvent(Event_Data_Stop_Server_Request::g_EventType);
	}
	else
	{
		URHO3D_LOGINFO("Could not stop server. Server was not created.");
	}
}

void MainMenuUI::HandleRestartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	if (g_pApp->GetGameLogic()->IsServerCreated())
	{
		SendEvent(String("Request_Restart_Server"));
	}
	else
	{
		URHO3D_LOGINFO("Could not restart server. Server was not created.");
	}
}


void MainMenuUI::HandlePauseServerDelegate(StringHash eventType, VariantMap& eventData)
{
	if (g_pApp->GetGameLogic()->IsServerCreated())
	{
		SendEvent(String("Request_Pause_Server"));
	}
	else
	{
		URHO3D_LOGINFO("Could not pause server. Server was not created.");
	}
}


void MainMenuUI::HandleServerOptionDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pServerInfoWindow->SetPriority(1);
	m_pPlayersInfoWindow->SetPriority(1);
	m_pServerOptionWindow->SetPriority(3);

	m_pServerInfoWindow->SetEnabled(false);
	m_pPlayersInfoWindow->SetEnabled(false);
	m_pServerOptionWindow->SetEnabled(true);
}

void MainMenuUI::HandlePlayersInfoDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pServerInfoWindow->SetPriority(1);
	m_pPlayersInfoWindow->SetPriority(3);
	m_pServerOptionWindow->SetPriority(1);

	m_pServerInfoWindow->SetEnabled(false);
	m_pPlayersInfoWindow->SetEnabled(true);
	m_pServerOptionWindow->SetEnabled(false);
}

void MainMenuUI::HandleServerInfoDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pServerInfoWindow->SetPriority(3);
	m_pPlayersInfoWindow->SetPriority(1);
	m_pServerOptionWindow->SetPriority(1);

	m_pServerInfoWindow->SetEnabled(true);
	m_pPlayersInfoWindow->SetEnabled(false);
	m_pServerOptionWindow->SetEnabled(false);
}

void MainMenuUI::HandleCloseDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pControlPanel->SetVisible(false);
}

void MainMenuUI::HandleServerCreatedDelegate(StringHash eventType, VariantMap& eventData)
{
	GameOptions& option = g_pApp->GetGameOptions();

	Text* serverStatus = (Text*)m_pControlPanel->GetChild("Text_Server_Status_Value", true);
	serverStatus->SetText("Running");
	serverStatus->SetColor(Color(0.0f, 1.0f, 0.1f));

	Text* serverAddress = (Text*)m_pControlPanel->GetChild("Text_Server_Address_Value", true);
	serverAddress->SetText(option.m_GameHost);
	serverAddress->SetColor(Color(0.0f, 1.0f, 0.1f));

	Text* serverPort = (Text*)m_pControlPanel->GetChild("Text_Server_Port_Value", true);
	serverPort->SetText(String(option.m_ListenPort));
	serverPort->SetColor(Color(0.0f, 1.0f, 0.1f));

	Text* serverName = (Text*)m_pControlPanel->GetChild("Text_Server_Name_Value", true);
	serverName->SetText(String("Hangars #1 Server"));
	serverName->SetColor(Color(0.0f, 1.0f, 0.1f));
}

void MainMenuUI::HandleServerStoppedDelegate(StringHash eventType, VariantMap& eventData)
{
	GameOptions& option = g_pApp->GetGameOptions();

	Text* serverStatus = (Text*)m_pControlPanel->GetChild("Text_Server_Status_Value", true);
	serverStatus->SetText("Stopped");
	serverStatus->SetColor(Color(1.0f, 0.0f, 0.1f));
}