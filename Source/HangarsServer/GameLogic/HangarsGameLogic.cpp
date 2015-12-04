#include <HangarsServerStd.h>
#include "EngineStd/EventManager/Server/ServerEvents.h"
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

		case BGS_ServerStopped:
		{
	
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
			GameOptions& gameOption = g_pApp->GetGameOptions();
			// There we should detect server ip address
			gameOption.m_GameHost = "localhost";
			
			// Send event to subsystems about server creation result.
			Event_Data_Server_Create_Result serverResultEvent(true);
			VariantMap data = serverResultEvent.VSerialize();
			SendEvent(Event_Data_Server_Create_Result::g_EventType, data);

			VChangeState(BGS_ServerCreated);
			URHO3D_LOGINFO("Server state is Created");
		}
		else
		{
			// Send event to subsystems about server creation result.
			Event_Data_Server_Create_Result serverResultEvent(false);
			VariantMap data = serverResultEvent.VSerialize();
			SendEvent(Event_Data_Server_Create_Result::g_EventType, data);

			VChangeState(BGS_Invalid);
			URHO3D_LOGINFO("Server state is Invalid");
		}
	}
	else if (m_State == BGS_WaitingForServerStop)
	{
		g_pApp->DestroyNetwork();

		// Send event to subsystems about server stopping.
		Event_Data_Server_Stop_Result serverStopEvent;
		SendEvent(Event_Data_Server_Stop_Result::g_EventType);

		VChangeState(BGS_ServerStopped);
		URHO3D_LOGINFO("Server state is Stopped");
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

	SubscribeToEvent(Event_Data_Start_Server_Request::g_EventType, URHO3D_HANDLER(HangarsGameLogic, StartServerDelegate));
	SubscribeToEvent(Event_Data_Stop_Server_Request::g_EventType, URHO3D_HANDLER(HangarsGameLogic, StopServerDelegate));
	//SubscribeToEvent("Request_Restart_Server", URHO3D_HANDLER(HangarsGameLogic, RestartServerDelegate));
	//sSubscribeToEvent("Request_Pause_Server", URHO3D_HANDLER(HangarsGameLogic, PauseServerDelegate));
}

void HangarsGameLogic::VDestroyAllDelegates()
{
	BaseGameLogic::VDestroyAllDelegates();
}

void HangarsGameLogic::StartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	VChangeState(BGS_WaitingForServerCreating);
}

void HangarsGameLogic::StopServerDelegate(StringHash eventType, VariantMap& eventData)
{
	VChangeState(BGS_WaitingForServerStop);
}

void HangarsGameLogic::RestartServerDelegate(StringHash eventType, VariantMap& eventData)
{
	//VChangeState(BGS_WaitingForServerRestarting);
}

void HangarsGameLogic::PauseServerDelegate(StringHash eventType, VariantMap& eventData)
{
	//VChangeState(BGS_WaitingForServerPause);
}