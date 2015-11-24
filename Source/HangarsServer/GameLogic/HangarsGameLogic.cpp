#include <HangarsServerStd.h>
#include "EngineStd/Network/Managers/BaseSocketManager.h"
#include "EngineStd/Network/Sockets/GameServerListenSocket.h"
#include "HangarsGameLogic.h"


HangarsGameLogic::HangarsGameLogic(Context* context) : BaseGameLogic(context)
{


}

HangarsGameLogic::~HangarsGameLogic()
{


}

bool HangarsGameLogic::VInitialize()
{
	BaseGameLogic::VInitialize();

	return true;
}


void HangarsGameLogic::VShutdown()
{
	BaseGameLogic::VShutdown();
}


void HangarsGameLogic::VChangeState(enum BaseGameState newState)
{
	BaseGameLogic::VChangeState(newState);
	if (newState == BGS_WaitingForPlayers)
	{
		BaseSocketManager *pServer = new BaseSocketManager(context_);
		if (!pServer->Init())
		{
			// Throw up a main menu
			VChangeState(BGS_MainMenu);
			return;
		}
		 
		pServer->AddSocket(new GameServerListenSocket(context_, g_pApp->GetGameOptions().m_ListenPort));
		g_pApp->SetSocketManager(pServer);
	}
}

bool HangarsGameLogic::VLoadGameDelegate(String pLevelData)
{
	return true;
}