#include "EngineStd.h"

#include <iostream>

using namespace std;

#include "GameAsset.h"
#include "GameAssetRules.h"
#include "GameAssetManager.h"

// constructor - initialize set default
GameAssetManager::GameAssetManager(Context* context)
    :LogicComponent(context)
	,m_pGameAssetLibrary(NULL)
	,m_pGameAssetRuleLibrary(NULL)
	,m_pGameAssetResources(NULL)

{
    // GameAssetLibrary
    m_pGameAssetLibrary = new Vector<GameAsset*>();
    m_pGameAssetRuleLibrary = new Vector<GameAssetRule*>();

}

/// Register Subsystem
void GameAssetManager::RegisterNewSubsystem(Context* context)
{
    context -> RegisterSubsystem(new GameAssetManager(context));

    return;
}

// setup asset path
void GameAssetManager::Init(void)
{
    // get file system
    FileSystem* filesystem = GetSubsystem<FileSystem>();

    m_pGameAssetResources = new GameAssetData(context_);

    // Sets Path
    m_pGameAssetResources->SetGameAssetsPath(filesystem->GetProgramDir().CString());

    // Set Data filename
    m_pGameAssetResources->SetAddDataFile("GameAssets.pak");

    return;
}

// add game asset - base symbol
GameAsset * GameAssetManager::AddGameAsset(String GA_Name, String GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!m_pGameAssetLibrary)
    {
        return NULL;
    }

    // check valid
    if(GA_Name.Empty() || GA_Symbol.Empty())
    {
        return NULL;
    }

    // check if state or type is valid
    if(GA_Type == GAType_None || GA_State==GAState_None)
    {
        return NULL;
    }

    // create new game asset
    GameAsset * newGameAsset;

    newGameAsset = new GameAsset(context_);

    newGameAsset ->SetSymbol(GA_Symbol);
    newGameAsset ->SetTypeState(GA_Type, GA_State);

    // add to library
   m_pGameAssetLibrary->Push(newGameAsset);


    return newGameAsset;
}

// find a child
GameAsset * GameAssetManager::FindGameAssetByKeyword(String Keyword, bool useName=0)
{

    // if asset library is null
    if(!m_pGameAssetLibrary || m_pGameAssetLibrary->Size()==0)
    {
        return NULL;
    }

    // check valid
    if(Keyword.Empty())
    {
        return NULL;
    }

    for(unsigned int i=0; i< m_pGameAssetLibrary->Size(); i++)
    {
        if(useName == true ? m_pGameAssetLibrary->At(i)->GetName()== Keyword : m_pGameAssetLibrary->At(i)->GetSymbol() == Keyword)
        {
            return m_pGameAssetLibrary->At(i);
        }
    }


    return NULL;
}

// get game asset
GameAsset * GameAssetManager::GetGameAssetByIdx(unsigned int idx)
{
    // if asset library is null
    if(!m_pGameAssetLibrary || m_pGameAssetLibrary->Size()==0)
    {
        return NULL;
    }

    // test size
    if(idx<0 || idx>m_pGameAssetLibrary->Size())
    {
        return NULL;
    }

    return m_pGameAssetLibrary->At(idx);
}

// Get total amount of assets in library
unsigned int GameAssetManager::GetTotalGameAssets(void)
{
    // if asset library is null
    if(!m_pGameAssetLibrary || m_pGameAssetLibrary->Size()==0)
    {
        return 0;
    }

    return m_pGameAssetLibrary->Size();
}

// wipe game asset from memory
bool GameAssetManager::DeleteGameAsset(GameAsset * RemoveGameAsset)
{
    // if asset library is null
    if(!m_pGameAssetLibrary||m_pGameAssetLibrary->Size()==0)
    {
        return false;
    }

    // loop through library
	for (unsigned int i = 0; i < m_pGameAssetLibrary->Size(); i++)
    {
		if (m_pGameAssetLibrary->At(i) == RemoveGameAsset)
        {
            // Remove all Game Assets
			m_pGameAssetLibrary->At(i)->RemoveClean();

            // remove from library
			m_pGameAssetLibrary->Erase(m_pGameAssetLibrary->Begin() + i);

            // remove from memory
            //delete RemoveGameAsset;
        }
    }

    return true;
}

// deconstructor
GameAssetManager::~GameAssetManager()
{
    // delete game assets from memory
	SAFE_DELETE(m_pGameAssetLibrary);
	SAFE_DELETE(m_pGameAssetRuleLibrary);

    // delete rest
	SAFE_DELETE(m_pGameAssetResources);

}
