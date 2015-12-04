#pragma once

//---------------------------------------------------------------------------------------------------------------------
// class Event_Data_Request_New_Game_Node			
// This event is sent by a server asking Client proxy logics to create new game node from their local resources.
// It can be sent from script or via code.
// This event is also sent from the server game logic to client logics AFTER it has created a new game node. The logics will allow follow suit to stay in sync.
// So, client has to subsribed to this event.
//---------------------------------------------------------------------------------------------------------------------

#include "../BaseEvent.h"

class Event_Data_Request_New_Game_Node : public BaseEventData
{
	URHO3D_OBJECT(Event_Data_Request_New_Game_Node, BaseEventData);
public:
	static const EventType g_EventType;

	Event_Data_Request_New_Game_Node() :
		BaseEventData()
	{
		m_GameAssetName = "";
		m_isInitialTransform = false;
		m_InitialTransform = Matrix4::IDENTITY;
		m_ServerGameNodeId = INVALID_GAME_NODE_ID;
		m_ViewId = INVALID_GAME_VIEW_ID;
	}

	Event_Data_Request_New_Game_Node(const String gameAssetName, const Matrix4* initMatrix = NULL, const GameNodeId nodeId = INVALID_GAME_NODE_ID, const GameViewId viewId = INVALID_GAME_VIEW_ID) :
		BaseEventData()
	{
		m_GameAssetName = gameAssetName;
		m_ViewId = viewId;
		m_ServerGameNodeId = nodeId;

		if (initMatrix)
		{
			m_isInitialTransform = true;
			m_InitialTransform = (*initMatrix);
		}
		else
		{
			m_InitialTransform = Matrix4::IDENTITY;
			m_isInitialTransform = false;
		}
	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["GAME_ASSET_NAME"] = m_GameAssetName;
		m_OutSerializer["GAME_VIEW_ID"] = m_ViewId;
		m_OutSerializer["SERVER_GAME_NODE_ID"] = m_ServerGameNodeId;
		m_OutSerializer["IS_INITIAL_TRANSFORM"] = m_isInitialTransform;
		m_OutSerializer["INITIAL_TRANSFORM"] = m_InitialTransform;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_GameAssetName = in["GAME_ASSET_NAME"].GetString();
		m_ViewId = in["GAME_VIEW_ID"].GetUInt();
		m_ServerGameNodeId = in["SERVER_GAME_NODE_ID"].GetUInt();
		m_isInitialTransform = in["IS_INITIAL_TRANSFORM"].GetBool();
		m_InitialTransform = in["INITIAL_TRANSFORM"].GetMatrix4();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Request_New_Game_Node(m_GameAssetName, (m_isInitialTransform) ? &m_InitialTransform : NULL, m_ServerGameNodeId, m_ViewId));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	const String& GetGameAsset(void) const { return m_GameAssetName; }
	const Matrix4* GetInitialTransform(void) const { return (m_isInitialTransform) ? &m_InitialTransform : NULL; }
	const GameNodeId GetServerActorId(void) const { return m_ServerGameNodeId; }
	GameViewId GetViewId(void) const { return m_ViewId; }

protected:
	String m_GameAssetName;
	GameViewId m_ViewId;
	GameNodeId m_ServerGameNodeId;
	bool m_isInitialTransform;
	Matrix4 m_InitialTransform;
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Network_Player_Game_Node_Assignment - sent by the server to the clients when a network view is assigned a player number
//---------------------------------------------------------------------------------------------------------------------
class Event_Data_Network_Player_Game_Node_Assignment : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Network_Player_Game_Node_Assignment(void)
	{
		m_GameNodeId = INVALID_GAME_NODE_ID;
		m_ConnectionId = INVALID_CONNECTION_ID;
	}

	explicit Event_Data_Network_Player_Game_Node_Assignment(const GameNodeId gameNodeId, const int connectionId) :
		m_GameNodeId(gameNodeId), 
		m_ConnectionId(connectionId)
	{

	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["GAME_NODE_ID"] = m_GameNodeId;
		m_OutSerializer["CONNECTION_ID"] = m_ConnectionId;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_GameNodeId = in["GAME_NODE_ID"].GetUInt();
		m_ConnectionId = in["CONNECTION_ID"].GetUInt();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Network_Player_Game_Node_Assignment(m_GameNodeId, m_ConnectionId));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	GameNodeId GetGameNodeId(void) const { return m_GameNodeId; }
	GameViewId GetConnectionId(void) const { return m_ConnectionId; }

protected:
	GameNodeId m_GameNodeId;
	int m_ConnectionId;

};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Network_Player_Login - sent by the server to the clients about logging success
//---------------------------------------------------------------------------------------------------------------------
class Event_Data_Network_Player_Login_Result : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Network_Player_Login_Result()
	{
		m_bIsSuccess = false;
		m_Reason = String("UNKNOWN");
	}

	explicit Event_Data_Network_Player_Login_Result(bool success, String reason) :
		m_bIsSuccess(success),
		m_Reason(reason)
	{

	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["SERVER_SUCCESS"] = m_bIsSuccess;
		m_OutSerializer["REASON"] = m_Reason;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_bIsSuccess = in["SERVER_SUCCESS"].GetBool();
		m_Reason = in["REASON"].GetString();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Network_Player_Login_Result(m_bIsSuccess, m_Reason));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	bool GetSuccess(void) const { return m_bIsSuccess; }
	String GetReason(void) const { return m_Reason; }

	void SetSuccess(bool value) { m_bIsSuccess = value; }
	void SetReason(String reason) { m_Reason = reason; }

protected:
	bool m_bIsSuccess;
	String m_Reason;
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Server_Create_Result - sent by the server when server is created or not
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Server_Create_Result : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Server_Create_Result()
	{
		m_bIsSuccess = false;
	}

	explicit Event_Data_Server_Create_Result(bool success) : m_bIsSuccess(success)
	{

	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["LOGIN_SUCCESS"] = m_bIsSuccess;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_bIsSuccess = in["LOGIN_SUCCESS"].GetBool();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Server_Create_Result(m_bIsSuccess));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	bool GetSuccess(void) const { return m_bIsSuccess; }
	void SetSuccess(bool value) { m_bIsSuccess = value; }
	

protected:
	bool m_bIsSuccess;
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Server_Stop - sent by the server when server is stopped
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Server_Stop_Result : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Server_Stop_Result()
	{
	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Server_Stop_Result());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Start_Server_Request - sent by the server when want to start server (create server)
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Start_Server_Request : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Start_Server_Request()
	{
	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Start_Server_Request());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Stop_Server_Request - sent by the server when want to stop server 
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Stop_Server_Request : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Stop_Server_Request()
	{
	}

	virtual const VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Stop_Server_Request());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};


