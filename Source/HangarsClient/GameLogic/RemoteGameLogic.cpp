#include "HangarsClientStd.h"
#include "RemoteGameLogic.h"

#include "EventManager/Server/ServerEvents.h"

RemoteGameLogic::RemoteGameLogic(Context* context) : BaseGameLogic(context)
{


}

RemoteGameLogic::~RemoteGameLogic()
{


}

bool RemoteGameLogic::VInitialize()
{
    BaseGameLogic::VInitialize();

    return true;
}

void RemoteGameLogic::VShutdown()
{
    BaseGameLogic::VShutdown();

}

void RemoteGameLogic::VOnUpdate(float timeStep)
{
	BaseGameLogic::VOnUpdate(timeStep);

	switch (m_State)
	{
		case BGS_WaitingForPlayer:
		{
			if (m_bIsPlayerLoggedIn)
			{
				VChangeState(BGS_LoadingPlayerLobby);
			}
			break;
		}

		case BGS_LoadingPlayerLobby:
		{
			break;
		}
	}
}


void RemoteGameLogic::VChangeState(enum BaseGameState newState)
{
    BaseGameLogic::VChangeState(newState);

	if (newState == BGS_WaitingForPlayer)
	{
		VSetProxy();
		g_pApp->AttachAsClient();
	}
}

bool RemoteGameLogic::VLoadGameDelegate(pugi::xml_node pLevelData)
{
    return true;
}

void RemoteGameLogic::HandleLoginRequest(StringHash eventType, VariantMap& eventData)
{
	String reason = eventData["REASON"].GetString();
	bool success = eventData["LOGIN_SUCCESS"].GetBool();

	if (!success)
	{
		g_pApp->DestroyNetwork();
		URHO3D_LOGERROR("Failed to login. Reason : " + reason);
	}

	SetLoginSuccess(success, reason);
}

void RemoteGameLogic::VSetProxy()
{
	BaseGameLogic::VSetProxy();

	SubscribeToEvent(Event_Data_Network_Player_Login_Result::g_EventType, URHO3D_HANDLER(RemoteGameLogic, HandleLoginRequest));
	g_pApp->GetNetwork()->RegisterRemoteEvent(Event_Data_Network_Player_Login_Result::g_EventType);
}