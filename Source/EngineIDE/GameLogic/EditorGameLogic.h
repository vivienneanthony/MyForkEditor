#ifndef ENGINE_GAME_LOGIC_H
#define ENGINE_GAME_LOGIC_H

#include "EngineStd/GameLogic/BaseGameLogic.h"

class EditorGameLogic : public BaseGameLogic
{
	URHO3D_OBJECT(EditorGameLogic, BaseGameLogic)
public:
	EditorGameLogic(Context* context);
	virtual ~EditorGameLogic();


public:
	virtual bool VInitialize();
	virtual void VShutdown();

	virtual void VChangeState(enum BaseGameState newState);

	virtual bool VLoadGameDelegate(String pLevelData);

};

#endif // ENGINE_GAME_LOGIC_H
