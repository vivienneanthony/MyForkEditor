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

    GameAsset * SphereTest = m_pGameAssetManager->FindGameAssetBySymbol("SphereTest");

    // Test if sphere was loaded
    if(SphereTest)
    {
        URHO3D_LOGINFO("A sphere loaded");
    }

    // Attempt to load a test sphere
    m_pGameAssetFactory->CreateNode(SphereTest, 0);

    // attempt to add child to scene
    //m_pScene ->AddChild(SphereTestNode);

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
