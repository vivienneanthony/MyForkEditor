#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// a physics implementation which does nothing.  used if physics is disabled.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class NullPhysics : public IGamePhysics
{
	URHO3D_OBJECT(NullPhysics, IGamePhysics)

public:
	NullPhysics(Context* context) : IGamePhysics(context) { }
	virtual ~NullPhysics() { }

	// Initialization and Maintenance of the Physics World
	virtual bool VInitialize() { return true; }
	virtual void VSyncVisibleScene() { };
	virtual void VOnUpdate(float timeStep) { }
	// Debugging
	virtual void VRenderDiagnostics() { }
};