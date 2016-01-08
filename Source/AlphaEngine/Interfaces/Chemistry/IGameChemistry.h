#pragma once

class IGameChemistry : public Object
{
public:
	IGameChemistry(Context* context);
	virtual ~IGameChemistry();

	// Initialiazation and Maintenance 
	virtual bool VInitialize() = 0;
	virtual void VOnUpdate(float timeStep) = 0;

};