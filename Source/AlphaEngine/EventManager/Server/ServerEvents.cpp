#include "AlphaEngineStd.h"
#include "ServerEvents.h"

// Server events
const EventType Event_Data_Request_New_Game_Node::g_EventType(StringHash(String("Event_Data_Request_New_Game_Node")));
const EventType Event_Data_Network_Player_Game_Node_Assignment::g_EventType(StringHash(String("Event_Data_Network_Player_Game_Node_Assignment")));

// Login check
const EventType Event_Data_Network_Player_Login_Result::g_EventType(StringHash(String("Event_Data_Network_Player_Login_Result")));

// Server navigation events (requests/result)
// Start server
const EventType Event_Data_Start_Server_Request::g_EventType(StringHash(String("Event_Data_Start_Server_Request")));
const EventType Event_Data_Server_Create_Result::g_EventType(StringHash(String("Event_Data_Server_Create_Result")));

// Stop server
const EventType Event_Data_Stop_Server_Request::g_EventType(StringHash(String("Event_Data_Stop_Server_Request")));
const EventType Event_Data_Server_Stop_Result::g_EventType(StringHash(String("Event_Data_Server_Stop_Result")));


