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
	virtual bool VInitialize();
	virtual void VShutdown();

	virtual void VChangeState(enum BaseGameState newState);

	virtual bool VLoadGameDelegate(String pLevelData);
};

#endif // HANGARS_GAME_LOGIC_H
