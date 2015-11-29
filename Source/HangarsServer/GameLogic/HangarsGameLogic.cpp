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

void HangarsGameLogic::VOnUpdate(float timeStep)
{
	BaseGameLogic::VOnUpdate(timeStep);

	switch (m_State)
	{

		case BGS_ServerCreated:
		{

			VChangeState(BGS_Running);
			break;
		}

	}

}


void HangarsGameLogic::VChangeState(enum BaseGameState newState)
{
	
	BaseGameLogic::VChangeState(newState);

	if (m_State == BGS_WaitingForServerCreating)
	{
		BaseSocketManager *pServer = new BaseSocketManager(context_);
		if (!pServer->Init())
		{
			// Throw up a main menu
			VChangeState(BGS_MainMenu);
			return;
		}

		NetSocket* socket = new GameServerListenSocket(context_, g_pApp->GetGameOptions().m_ListenPort);
		
		pServer->AddSocket(socket);
		g_pApp->SetSocketManager(pServer);

		if (m_bIsServerCreated)
		{
			VChangeState(BGS_ServerCreated);
		}
		else
		{
			VChangeState(BGS_Invalid);
		}
	}
	else if (m_State == BGS_Invalid)
	{
		URHO3D_LOGERROR("Server state is INVALID !");
	}
}

bool HangarsGameLogic::VLoadGameDelegate(String pLevelData)
{
	return true;
}


void HangarsGameLogic::VInitializeAllDelegates()
{
	BaseGameLogic::VInitializeAllDelegates();

	SubscribeToEvent("Request_Start_Server", URHO3D_HANDLER(HangarsGameLogic, StartServerDelegate));
	
}

void HangarsGameLogic::VDestroyAllDelegates()
{
	BaseGameLogic::VDestroyAllDelegates();
}

void HangarsGameLogic::StartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	VChangeState(BGS_WaitingForServerCreating);
}