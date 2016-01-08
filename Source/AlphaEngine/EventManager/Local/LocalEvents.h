#pragma once
#include "../BaseEvent.h"


//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Stop_Introduction_Part - This event is sent out when introduction like Logo show is ended.
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Finish_Introduction_Part : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Finish_Introduction_Part()
	{
	}


	virtual VariantMap& VSerialize()
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
		return IEventDataPtr(new Event_Data_Finish_Introduction_Part());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_New_GameNode - This event is sent out when a game node is *actually* created.
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_New_Game_Node : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_New_Game_Node(void)
	{
		m_GameNodeId = INVALID_GAME_NODE_ID;
		m_ViewId = INVALID_GAME_VIEW_ID;
	}

	explicit Event_Data_New_Game_Node(GameNodeId actorId, GameViewId viewId = INVALID_GAME_VIEW_ID) :
		m_GameNodeId(actorId),
		m_ViewId(viewId)
	{

	}

	virtual VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["GAME_VIEW_ID"] = m_ViewId;
		m_OutSerializer["GAME_NODE_ID"] = m_GameNodeId;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_GameNodeId = in["GAME_ASSET_NAME"].GetUInt();
		m_ViewId = in["GAME_VIEW_ID"].GetUInt();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_New_Game_Node(m_GameNodeId, m_ViewId));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	GameNodeId GetGameNodeId(void) const { return m_GameNodeId; }
	GameViewId GetViewId(void) const { return m_ViewId; }

protected:
	GameNodeId m_GameNodeId;
	GameViewId m_ViewId;

};

//---------------------------------------------------------------------------------------------------------------------
// EvtData_Destroy_Actor - sent when game node are destroyed	
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Destroy_Game_Node : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Destroy_Game_Node(void)
	{
		m_GameNodeId = INVALID_GAME_NODE_ID;
	}

	explicit Event_Data_Destroy_Game_Node(GameNodeId actorId) :
		m_GameNodeId(actorId)
	{

	}

	virtual VariantMap& VSerialize()
	{
		m_OutSerializer["EVENT_TYPE"] = g_EventType.ToHash();
		m_OutSerializer["EVENT_ORDER"] = m_EventOrder;
		m_OutSerializer["GAME_NODE_ID"] = m_GameNodeId;
		return m_OutSerializer;
	}

	virtual void VDeserialize(VariantMap& in)
	{
		m_EventOrder = in["EVENT_ORDER"].GetBool();
		m_GameNodeId = in["GAME_NODE_ID"].GetUInt();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Destroy_Game_Node(m_GameNodeId));
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}

	GameNodeId GetGameNodeId(void) const { return m_GameNodeId; }

protected:
	GameNodeId m_GameNodeId;
};