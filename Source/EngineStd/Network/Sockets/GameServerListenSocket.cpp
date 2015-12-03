#include "EngineStd.h"
#include "EventManager/Events.h"
#include "GameServerListenSocket.h"

GameServerListenSocket::GameServerListenSocket(Context* context, int portNum) : NetListenSocket(context, portNum)
{
	VInitializeAllDelegates();
}


void GameServerListenSocket::VInitializeAllDelegates()
{
	//SubscribeToEvent(E_CLIENTCONNECTED, URHO3D_HANDLER(GameServerListenSocket, HandleClientConnection));
	SubscribeToEvent(E_CLIENTIDENTITY, URHO3D_HANDLER(GameServerListenSocket, HandleClientIdentity));
}


void GameServerListenSocket::VDestroyAllDelegates()
{
	
}

void GameServerListenSocket::HandleClientConnection(StringHash eventType, VariantMap& eventData)
{
	Connection* connection = (Connection*)eventData[ClientConnected::P_CONNECTION].GetPtr();
	String address = connection->GetAddress();

}

void GameServerListenSocket::HandleClientIdentity(StringHash eventType, VariantMap& eventData)
{
	Connection* connection = (Connection*)eventData[ClientIdentity::P_CONNECTION].GetPtr();
	bool allow = eventData[ClientIdentity::P_ALLOW].GetBool();
	
	String address = connection->GetAddress();
	
	String login = eventData["CLIENT_LOGIN"].GetString();
	String password = eventData["CLIENT_PASSWORD"].GetString();

	VariantMap data;
	Event_Data_Network_Player_Login_Result loginEvent;

	// Here should be check for password/login identify from databases		
	if (login == String("admin") && password == String("admin"))
	{
		loginEvent.SetSuccess(true);
	}
	else 
	{
		loginEvent.SetSuccess(false);
		loginEvent.SetReason("Invalid_login_data");
	}

	data = loginEvent.VSerialize();
	connection->SendRemoteEvent(Event_Data_Network_Player_Login_Result::g_EventType, loginEvent.GetEventOrder(), data);
}