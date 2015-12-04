#include "HangarsClientStd.h"

#include "EngineStd/UserInterface/UserInterface.h"
#include "EngineStd/UserInterface/Urho3D/Utilities.h"


#include "EngineStd/EventManager/Client/ClientEvents.h"

#include "EngineStd/GameLogic/BaseGameLogic.h"
#include "EngineStd/GameLogic/LevelManager/LevelManager.h"
#include "EngineStd/Resources/ResHandle.h"
#include "EngineStd/Resources/ResourceCache.h"

#include "EngineStd/GameAssetManager/GameAssetManager.h"

#include "MainMenuUI.h"

#include <iostream>

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

        CreateLoginWindow();
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
    m_pMainMenuScene->RemoveChild(m_pMusicNode);
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

void MainMenuUI::CreateLoginWindow()
{
    m_pWindow = CreateCustomWindow(context_, "LoginWindow", IntVector2(0, -50));

    Text* textName = new Text(context_);
    textName->SetText("Enter your login: ");

    textName->SetTextAlignment(HorizontalAlignment::HA_CENTER);
    textName->SetStyleAuto();

    m_pWindow->AddChild(textName);
    m_pWindow->SetFixedHeight(300);
    m_pWindow->SetPosition(0, -100);

    // Create a LoginEdit
    m_pLoginEdit = new LineEdit(context_);
    m_pLoginEdit->SetName("LoginEdit");
    m_pLoginEdit->SetMinHeight(24);
    m_pLoginEdit->SetStyleAuto();
    m_pLoginEdit->SetFixedWidth(350);
    m_pLoginEdit->SetFixedHeight(20);
    m_pLoginEdit->SetAlignment(HorizontalAlignment::HA_CENTER, VA_TOP);
    m_pWindow->AddChild(m_pLoginEdit);

    Text* textPasswordName = new Text(context_);
    textPasswordName->SetText("Enter your password: ");

    textPasswordName->SetTextAlignment(HorizontalAlignment::HA_CENTER);
    textPasswordName->SetStyleAuto();

    m_pWindow->AddChild(textPasswordName);

    // Create a LoginEdit
    m_pPasswordEdit = new LineEdit(context_);
    m_pPasswordEdit->SetName("PasswordEdit");
    m_pPasswordEdit->SetMinHeight(24);
    m_pPasswordEdit->SetStyleAuto();
    m_pPasswordEdit->SetFixedWidth(350);
    m_pPasswordEdit->SetFixedHeight(20);
    m_pPasswordEdit->SetAlignment(HorizontalAlignment::HA_CENTER, VA_TOP);
    m_pWindow->AddChild(m_pPasswordEdit);

    Button* enterButton = CreateCustomButton(m_pWindow, "Enter", "Enter");
    SubscribeToEvent(enterButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, HandleEnterDelegate));

	SubscribeToEvent(Event_Data_Player_Login_Result::g_EventType, URHO3D_HANDLER(MainMenuUI, HandlePlayerLoginResult));

    // Set Opacity
    m_pWindow->SetOpacity(.5);

}

void MainMenuUI::HandleEnterDelegate(StringHash eventType, VariantMap& eventData)
{
    String loginName = m_pLoginEdit->GetText();
    String passwordName = m_pPasswordEdit->GetText();

	SendEvent(Event_Data_Request_Start_Game::g_EventType);

}


void MainMenuUI::HandleCloseDelegate(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<UI>()->GetRoot()->RemoveChild(m_pWindow);
    m_pWindow = nullptr;
}

void MainMenuUI::HandlePlayerLoginResult(StringHash eventType, VariantMap& eventData)
{
	Event_Data_Player_Login_Result logResult;
	logResult.VDeserialize(eventData);

	bool success = logResult.GetSuccess();

	if (success)
	{
		// This event not handled yet.
		SendEvent(Event_Data_Request_Enter_Lobby::g_EventType);
	}
	else
	{
		String reason = logResult.GetReason();
		// here we show message about login fail

	}
}