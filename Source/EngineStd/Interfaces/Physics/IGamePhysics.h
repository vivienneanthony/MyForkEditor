#pragma once

class IGamePhysics : public Object
{
public:
	IGamePhysics(Context* context);
	virtual ~IGamePhysics();

	// Initialiazation and Maintenance of the Physics World
	virtual bool VInitialize() = 0;
	virtual void VSyncVisibleScene() = 0;
	virtual void VOnUpdate(float timeStep) = 0;

	// Debugging
	virtual void VRenderDiagnostics() = 0;
};