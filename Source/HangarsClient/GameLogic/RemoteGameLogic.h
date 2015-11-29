#ifndef REMOTE_GAME_LOGIC_H
#define REMOTE_GAME_LOGIC_H

#include "EngineStd/GameLogic/BaseGameLogic.h"

class RemoteGameLogic : public BaseGameLogic
{
	URHO3D_OBJECT(RemoteGameLogic, BaseGameLogic)
public:
	RemoteGameLogic(Context* context);
	virtual ~RemoteGameLogic();


public:
	virtual bool VInitialize();
	virtual void VShutdown();


	virtual void VChangeState(enum BaseGameState newState);

	virtual bool VLoadGameDelegate(String pLevelData);

protected:


};

#endif // REMOTE_GAME_LOGIC_H
