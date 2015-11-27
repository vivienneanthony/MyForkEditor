#include "HangarsClientStd.h"

#include "GameLogic/RemoteGameLogic.h"

#include "GameView/HangarsHumanView.h"
#include "GameView/MainMenuView.h"

#include "HangarsClientApplication.h"



// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(HangarsClientApp)

HangarsClientApp::HangarsClientApp(Context* context) : EngineApp(context)
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
	EngineApp::Setup();


}


void HangarsClientApp::Start()
{
	EngineApp::Start();

	m_bIsInit = true;
}

void HangarsClientApp::Stop()
{
	EngineApp::Stop();

}

bool HangarsClientApp::CreateCursor()
{
	EngineApp::CreateCursor();

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
	EngineApp::VInitializeAllDelegates();

}

void HangarsClientApp::VDestroyAllDelegates()
{
	EngineApp::VDestroyAllDelegates();

}

