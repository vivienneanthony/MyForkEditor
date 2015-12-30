#ifndef IGAME_LOGIC_H
#define IGAME_LOGIC_H

class IGamePhysics;
class IGameChemistry;

enum BaseGameState : int;

class IGameLogic : public Object
{
public:
	IGameLogic(Context* context);
	virtual ~IGameLogic();

	virtual bool VInitialize() = 0;
	virtual void VOnUpdate(float timeStep) = 0;
	virtual void VShutdown() = 0;

	virtual void VDestroyGameNode(const GameNodeId nodeId) = 0;

	virtual void VChangeState(BaseGameState newState) = 0;
	virtual bool VLoadGame(String levelResource) = 0;

	virtual SharedPtr<IGamePhysics> VGetGamePhysics() = 0;
	virtual SharedPtr<IGameChemistry> VGetGameChemistry() = 0;
};

#endif //IGAME_LOGIC_H
