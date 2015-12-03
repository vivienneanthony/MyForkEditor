#include "EngineStd.h"
#include "Events.h"

// Local engine events
const EventType Event_Data_Request_New_Game_Node::g_EventType(StringHash(String("Event_Data_Request_New_Game_Node")));


// Network events
const EventType Event_Data_New_Game_Node::g_EventType(StringHash(String("Event_Data_New_Game_Node")));
const EventType Event_Data_Network_Player_Game_Node_Assignment::g_EventType(StringHash(String("Event_Data_Network_Player_Game_Node_Assignment")));
const EventType Event_Data_Network_Player_Login_Result::g_EventType(StringHash(String("Event_Data_Network_Player_Login")));

// Server events


