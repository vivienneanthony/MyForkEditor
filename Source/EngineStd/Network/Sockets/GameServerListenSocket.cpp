#include "EngineStd.h"
#include "GameServerListenSocket.h"

GameServerListenSocket::GameServerListenSocket(Context* context, int portNum) : NetListenSocket(context, portNum)
{


}


void GameServerListenSocket::VInitializeDelegates()
{
	NetListenSocket::VInitializeDelegates();

	SubscribeToEvent(E_CLIENTCONNECTED, URHO3D_HANDLER(GameServerListenSocket, HandleClientConnection));
	SubscribeToEvent(E_CLIENTIDENTITY, URHO3D_HANDLER(GameServerListenSocket, HandleClientIdentity));
}


void GameServerListenSocket::VDestroyAllDelegates()
{
	
}

void GameServerListenSocket::HandleClientConnection(StringHash eventType, VariantMap& eventData)
{
		


}

void GameServerListenSocket::HandleClientIdentity(StringHash eventType, VariantMap& eventData)
{


}