#include "HangarsServerStd.h"

#include "GameLogic/HangarsGameLogic.h"
#include "GameView/RemoteHangarsHumanView.h"
#include "GameView/MainMenuView.h"

#include "HangarsServerApplication.h"

// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(HangarsServerApp)

HangarsServerApp::HangarsServerApp(Context* context) : EngineApp(context)
{
	g_pApp = this;
	m_bIsInit = false;
	m_pCurrentCursor = nullptr;
	m_pBaseSocketManager = nullptr;
}

HangarsServerApp::~HangarsServerApp()
{

}


void HangarsServerApp::Setup()
{
	EngineApp::Setup();


}


void HangarsServerApp::Start()
{
	EngineApp::Start();

	m_bIsInit = true;
}

void HangarsServerApp::Stop()
{
	EngineApp::Stop();

}

bool HangarsServerApp::CreateCursor()
{
    EngineApp::CreateCursor();

	return true;
}

bool HangarsServerApp::VCreateViewLogic()
{
	m_pGameLogic = new HangarsGameLogic(context_);
	context_->RegisterSubsystem(m_pGameLogic);

	if (!m_pGameLogic->VInitialize())
	{
		URHO3D_LOGERROR("Game logic init failed");
		m_bIsInit = false;
	}
	URHO3D_LOGDEBUG("Game logic successfully initialized");

	SharedPtr<IGameView> menuView = SharedPtr<IGameView>(new MainMenuView(context_, nullptr, true));
	m_pGameLogic->VAddView(menuView);
	return true;
}

void HangarsServerApp::VInitializeAllDelegates()
{
	EngineApp::VInitializeAllDelegates();

}

void HangarsServerApp::VDestroyAllDelegates()
{
	EngineApp::VDestroyAllDelegates();

}

