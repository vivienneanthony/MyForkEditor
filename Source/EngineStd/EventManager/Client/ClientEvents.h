#pragma once

//---------------------------------------------------------------------------------------------------------------------
// Event_Data_Player_Login_Result - sent by the client to local subsystems (e.x. for mainmenu) that log is successful or not
//---------------------------------------------------------------------------------------------------------------------

#include "../BaseEvent.h"

class Event_Data_Player_Login_Result : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Player_Login_Result()
	{
		m_bIsSuccess = false;
		m_Reason = String("UNKNOWN");
	}

	explicit Event_Data_Player_Login_Result(bool success, String reason) :
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
		return IEventDataPtr(new Event_Data_Player_Login_Result(m_bIsSuccess, m_Reason));
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
// EvtData_Request_Start_Game - this is sent by the game client, in order to try to connect to the server
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Request_Start_Game : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Request_Start_Game()
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
		return IEventDataPtr(new Event_Data_Request_Start_Game());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};

//---------------------------------------------------------------------------------------------------------------------
// EvtData_Request_Start_Game - this is sent by the game client, in order to try to ask server about lobby info
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Request_Enter_Lobby : public BaseEventData
{
public:
	static const EventType g_EventType;

	Event_Data_Request_Enter_Lobby()
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
		return IEventDataPtr(new Event_Data_Request_Enter_Lobby());
	}

	virtual const String GetName(void) const
	{
		return g_EventType.ToString();
	}
};





