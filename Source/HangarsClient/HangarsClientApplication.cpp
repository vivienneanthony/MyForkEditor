#include "HangarsClientStd.h"

#include "GameLogic/RemoteGameLogic.h"

#include "GameView/HangarsHumanView.h"
#include "GameView/MainMenuView.h"

#include "HangarsClientApplication.h"



// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(HangarsClientApp)

HangarsClientApp::HangarsClientApp(Context* context) : AlphaEngineApp(context)
{
	g_pApp = this;
	m_bIsInit = false;
	m_pCurrentCursor = nullptr;
}

HangarsClientApp::~HangarsClientApp()
{

}


void HangarsClientApp::Setup()
{
	AlphaEngineApp::Setup();


}


void HangarsClientApp::Start()
{
	AlphaEngineApp::Start();

	m_bIsInit = true;
}

void HangarsClientApp::Stop()
{
	AlphaEngineApp::Stop();

}

bool HangarsClientApp::CreateCursor()
{
	AlphaEngineApp::CreateCursor();

	return true;
}

bool HangarsClientApp::VCreateViewLogic()
{
	m_pGameLogic = new RemoteGameLogic(context_);
	context_->RegisterSubsystem(m_pGameLogic);

	if (!m_pGameLogic->VInitialize())
	{
		URHO3D_LOGERROR("Game logic init failed");
		m_bIsInit = false;
	}
	URHO3D_LOGDEBUG("Game logic successfully initialized");


	SharedPtr<IGameView> menuView = SharedPtr<IGameView>(new MainMenuView(context_, m_pRenderer , true));
	m_pGameLogic->VAddView(menuView);

	return true;
}

void HangarsClientApp::VInitializeAllDelegates()
{
	AlphaEngineApp::VInitializeAllDelegates();

}

void HangarsClientApp::VDestroyAllDelegates()
{
	AlphaEngineApp::VDestroyAllDelegates();

}

