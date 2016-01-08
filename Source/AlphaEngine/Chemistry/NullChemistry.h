#pragma once

class NullChemistry : public IGameChemistry
{
	URHO3D_OBJECT(NullChemistry, IGameChemistry)
public:
	NullChemistry(Context* context) : IGameChemistry(context) { }
	virtual ~NullChemistry() { }

	// Initialiazation and Maintenance of the Physics World
	virtual bool VInitialize() { return true; }
	virtual void VOnUpdate(float timeStep) { }
};