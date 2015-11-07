#ifndef IGAME_LOGIC_H
#define IGAME_LOGIC_H

class IGamePhysics;
enum BaseGameState : int;

class IGameLogic : public Object
{
public:
	IGameLogic(Context* context);
	virtual ~IGameLogic();

	virtual bool VInitialize() = 0;
	virtual void VOnUpdate(float timeStep) = 0;
	virtual void VShutdown() = 0;

	virtual void VDestroyActor(const ActorId actorId) = 0;

	virtual void VChangeState(BaseGameState newState) = 0;
	virtual bool VLoadGame(String levelResource) = 0;
};

#endif //IGAME_LOGIC_H
