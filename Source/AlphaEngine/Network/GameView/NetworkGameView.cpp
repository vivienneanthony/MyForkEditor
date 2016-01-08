#include "AlphaEngineStd.h"

#include "Network/Managers/BaseSocketManager.h"
#include "Network/Packets/EventPacket.h"

#include "EventManager/Server/ServerEvents.h"
#include "EventManager/Local/LocalEvents.h"

#include "NetworkGameView.h"


NetworkGameView::NetworkGameView(Context* context) : IGameView(context)
{
	m_ConnectionId = INVALID_CONNECTION_ID;
	m_GameNodeId = INVALID_GAME_NODE_ID;

	VInitializeAllDelegates();
}

NetworkGameView::~NetworkGameView()
{

}

void NetworkGameView::VOnAttach(GameViewId vid, GameNodeId aid)
{

}

bool NetworkGameView::VOnRestore()
{
	return true;
}

bool NetworkGameView::VOnLostDevice()
{
	return true;
}

void NetworkGameView::VOnUpdate(float timeStep)
{

}

bool NetworkGameView::VOnMsgProc(AppMsg msg)
{
	return false;
}

void NetworkGameView::VShutdown()
{
	VDestroyAllDelegates();
}

void NetworkGameView::AttachRemotePlayer(ConnectionId connectionId)
{
	m_ConnectionId = connectionId;
	
	// this is the first thing that happens when the 
	// network view is attached. The socket id is sent, 
	// which is how each client can be uniquely identified from other
	// clients attached to the server.

	VariantMap data;
	Event_Data_Network_Player_Game_Node_Assignment eventData;
	eventData.VDeserialize(data);
	SharedPtr<BasePacket> packet(new EventPacket(context_, Event_Data_Network_Player_Game_Node_Assignment::g_EventType, eventData.GetEventOrder(), data));
	
	g_pSocketManager->Send(m_ConnectionId, packet);
}


void NetworkGameView::VInitializeAllDelegates()
{
	SubscribeToEvent(Event_Data_New_Game_Node::g_EventType, URHO3D_HANDLER(NetworkGameView, NewGameNodeDelegate));

}

void NetworkGameView::VDestroyAllDelegates()
{
	UnsubscribeFromAllEvents();
}

void NetworkGameView::NewGameNodeDelegate(StringHash eventType, VariantMap& eventData)
{

}
