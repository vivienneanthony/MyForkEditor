#include "HangarsClientStd.h"
#include "RemoteGameLogic.h"

#include "EngineStd/GameAssetManager/Factory/GameAssetFactory.h"
#include "EngineStd/GameAssetManager/GameAssetManager.h"

RemoteGameLogic::RemoteGameLogic(Context* context) : BaseGameLogic(context)
{


}

RemoteGameLogic::~RemoteGameLogic()
{


}

bool RemoteGameLogic::VInitialize()
{
    BaseGameLogic::VInitialize();

    return true;
}

void RemoteGameLogic::VShutdown()
{


    BaseGameLogic::VShutdown();
}


void RemoteGameLogic::VChangeState(enum BaseGameState newState)
{
    BaseGameLogic::VChangeState(newState);
}

bool RemoteGameLogic::VLoadGameDelegate(String pLevelData)
{
    return true;
}
