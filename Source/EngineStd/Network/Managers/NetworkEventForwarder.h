#pragma once


//
// class NetworkEventForwarder
//
// The only thing a network game view does is 
// act as an intermediary between the server and the 'real' remote view
// it listens to the same messages as a a game view
// and sends them along via TCP/IP

class NetworkEventForwarder : public Object
{
	URHO3D_OBJECT(NetworkEventForwarder, Object);

public:
	NetworkEventForwarder(unsigned int connectionId);

	// Delegate that forwards events through the network.  The game layer must register objects of this class for 
	// the events it wants.  See HangarsClientApp::VCreateGameAndView() and HangarsClientAp::RemoteClientDelegate()
	// for examples of this happening.
	void ForwardEventDelegate(StringHash eventType, VariantMap& eventData);

protected:
	unsigned int m_ConnectionId;

};
