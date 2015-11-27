#pragma once

// Events.h : defines common game events

//---------------------------------------------------------------------------------------------------------------------
// class BaseEventData	
//---------------------------------------------------------------------------------------------------------------------
class BaseEventData : public IEventData
{
public:
	explicit BaseEventData(const float timeStamp = 0.0f) : 
		 m_TimeStamp(timeStamp) 
		,m_OutSerializer(Variant::emptyVariantMap)
		,IEventData(g_pApp->GetContext())
	{
	}
		
	// Returns the type of the event
	virtual const EventType& VGetEventType(void) const = 0;

	float GetTimeStamp(void) const { return m_TimeStamp; }

	// Serializing for network input / output
	virtual const VariantMap& VSerialize() const { return m_OutSerializer; }
	virtual void VDeserialize(VariantMap& in) { }

protected:
	const float m_TimeStamp;

	VariantMap m_OutSerializer;
};

//---------------------------------------------------------------------------------------------------------------------
// class Event_Request_New_Game_Asset				
// This event is sent by a server asking Client proxy logics to create new game node from their local resources.
// It can be sent from script or via code.
// This event is also sent from the server game logic to client logics AFTER it has created a new game node. The logics will allow follow suit to stay in sync.
//---------------------------------------------------------------------------------------------------------------------

class Event_Data_Request_New_Game_Asset : public BaseEventData
{
	URHO3D_OBJECT(Event_Data_Request_New_Game_Asset, BaseEventData);
public:
	static const EventType g_EventType;

	Event_Data_Request_New_Game_Asset() : 
		BaseEventData()
	{
		m_GameAssetName = "";
		m_isInitialTransform = false;
		m_InitialTransform = Matrix4::IDENTITY;
		m_ServerGameNodeId = INVALID_GAME_NODE_ID;
		m_ViewId = INVALID_GAME_VIEW_ID;
	}

	Event_Data_Request_New_Game_Asset(String gameAssetName, Matrix4* initMatrix = NULL, GameNodeId nodeId = INVALID_GAME_NODE_ID, GameViewId viewId = INVALID_GAME_VIEW_ID) : 
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

	virtual const EventType& VGetEventType() const
	{
		return g_EventType;
	}

	virtual const VariantMap& VSerialize()
	{ 
		m_OutSerializer["GAME_ASSET_NAME"] = m_GameAssetName;
		m_OutSerializer["GAME_VIEW_ID"] = m_ViewId;
		m_OutSerializer["SERVER_GAME_NODE_ID"] = m_ServerGameNodeId;
		m_OutSerializer["IS_INITIAL_TRANSFORM"] = m_isInitialTransform;
		m_OutSerializer["INITIAL_TRANSFORM"] = m_InitialTransform;
		return m_OutSerializer;
	}
	
	virtual void VDeserialize(VariantMap& in) 
	{ 
		m_GameAssetName = in["GAME_ASSET_NAME"].GetString();
		m_ViewId = in["GAME_VIEW_ID"].GetUInt();
		m_ServerGameNodeId = in["SERVER_GAME_NODE_ID"].GetUInt();
		m_isInitialTransform = in["IS_INITIAL_TRANSFORM"].GetBool();
		m_InitialTransform = in["INITIAL_TRANSFORM"].GetMatrix4();
	}

	virtual IEventDataPtr VCopy()
	{
		return IEventDataPtr(new Event_Data_Request_New_Game_Asset(m_GameAssetName, (m_isInitialTransform) ? &m_InitialTransform : NULL, m_ServerGameNodeId, m_ViewId));
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