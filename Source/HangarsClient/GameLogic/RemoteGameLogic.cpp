#include "HangarsClientStd.h"
#include "RemoteGameLogic.h"

#include "EngineStd/GameAssetManager/Factory/GameAssetFactory.h"
#include "EngineStd/GameAssetManager/GameAssetManager.h"

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
