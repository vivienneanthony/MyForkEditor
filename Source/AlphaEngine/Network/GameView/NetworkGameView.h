#pragma once

class NetworkGameView : public IGameView
{
	URHO3D_OBJECT(NetworkGameView, IGameView)

public:
	NetworkGameView(Context* context);
	virtual ~NetworkGameView();

	// IGameView virtual methods
	virtual void VOnAttach(GameViewId vid, GameNodeId aid);
	virtual bool VOnRestore();
	virtual bool VOnLostDevice();
	virtual void VOnUpdate(float timeStep);
	virtual bool VOnMsgProc(AppMsg msg);
	virtual void VShutdown();

	// Network game view methods
	void AttachRemotePlayer(ConnectionId connectionId);

	// Getters/Setters
	virtual GameViewType VGetType() { return GameView_Remote; }
	virtual GameViewId VGetId() { return m_ViewId; }

	void SetGameNodeId(GameNodeId gameNodeId) { m_GameNodeId = gameNodeId; }
	int IsRemotePlayerAttached() { return m_ConnectionId != INVALID_CONNECTION_ID; }


	// Delegates
	virtual void VInitializeAllDelegates();			// Register all delegates
	virtual void VDestroyAllDelegates();			// Unsubscribe from all events
	
	void NewGameNodeDelegate(StringHash eventType, VariantMap& eventData);

protected:
	GameViewId m_ViewId;
	GameNodeId m_GameNodeId;

	int m_ConnectionId;
};

