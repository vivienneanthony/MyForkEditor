#ifndef HANGARS_GAME_LOGIC_H
#define HANGARS_GAME_LOGIC_H

#include <EngineStd/GameLogic/BaseGameLogic.h>

class HangarsGameLogic : public BaseGameLogic
{
	URHO3D_OBJECT(HangarsGameLogic,BaseGameLogic)
public:
	HangarsGameLogic(Context* context);
	virtual ~HangarsGameLogic();


public:
	virtual bool VInitialize() { return true; }
	virtual void VOnUpdate(float timeStep) { }
	virtual void VShutdown() { }

	virtual void VDestroyActor(const ActorId actorId) { }

	virtual void VChangeState(enum BaseGameState newState) { }

	virtual bool VLoadGameDelegate(String pLevelData) { return true; }
};

#endif // HANGARS_GAME_LOGIC_H
