#ifndef REMOTE_GAME_LOGIC_H
#define REMOTE_GAME_LOGIC_H

#include <EngineStd/GameLogic/BaseGameLogic.h>

class RemoteGameLogic : public BaseGameLogic
{
	URHO3D_OBJECT(RemoteGameLogic, BaseGameLogic)
public:
	RemoteGameLogic(Context* context);
	virtual ~RemoteGameLogic();


public:
	virtual bool VInitialize() { return true; }
	virtual void VOnUpdate(float timeStep) { }
	virtual void VShutdown() { }

	virtual void VDestroyActor(const ActorId actorId) { }

	virtual void VChangeState(enum BaseGameState newState) { }

	virtual bool VLoadGameDelegate(String pLevelData) { return true; }
};

#endif // REMOTE_GAME_LOGIC_H
