#include <iostream>
#include <vector>

using namespace std;

#include "EngineStd.h"

#include "GameAsset.h"
#include "GameAssetRules.h"
#include "GameAssetManager.h"

// constructor - initialize set default
GameAssetManager::GameAssetManager(Context *context)
    :LogicComponent(context)
    ,GameAssetLibrary(NULL)
    ,GameAssetRuleLibrary(NULL)

{
    // GameAssetLibrary
    GameAssetLibrary = new vector<GameAsset *>();
    GameAssetRuleLibrary =new vector<GameAssetRule *>();

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
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    GameAssetResources = new GameAssetData(context_);

    // Sets Path
    GameAssetResources -> SetGameAssetsPath(filesystem->GetProgramDir().CString());

    // Set Data filename
    GameAssetResources -> SetAddDataFile("GameAssets.pak");

    return;
}

// add game asset - base symbol
GameAsset * GameAssetManager::AddGameAsset(string GA_Name, string GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!GameAssetLibrary)
    {
        return NULL;
    }

    // check valid
    if(GA_Name.empty()||GA_Symbol.empty())
    {
        return NULL;
    }

    // check if state or type is valid
    if(GA_Type==GAType_None||GA_State==GAState_None)
    {
        return NULL;
    }

    // create new game asset
    GameAsset * newGameAsset;

    newGameAsset = new GameAsset(context_);

    newGameAsset ->SetSymbol(GA_Symbol);
    newGameAsset ->SetTypeState(GA_Type, GA_State);

    // add to library
    GameAssetLibrary->push_back(newGameAsset);


    return newGameAsset;
}

// find a child
GameAsset * GameAssetManager::FindGameAssetByKeyword(string Keyword, bool useName=0)
{

    // if asset library is null
    if(!GameAssetLibrary||GameAssetLibrary->size()==0)
    {
        return NULL;
    }

    // check valid
    if(Keyword.empty())
    {
        return NULL;
    }

    for(unsigned int i=0; i<GameAssetLibrary->size(); i++)
    {
        if(useName==true? GameAssetLibrary->at(i)->GetName()== Keyword : GameAssetLibrary->at(i)->GetSymbol()==Keyword)
        {
            return GameAssetLibrary->at(i);
        }
    }


    return NULL;
}

// get game asset
GameAsset * GameAssetManager::GetGameAssetByIdx(unsigned int idx)
{

    // if asset library is null
    if(!GameAssetLibrary||GameAssetLibrary->size()==0)
    {
        return NULL;
    }

    // test size
    if(idx<0||idx>GameAssetLibrary->size())
    {
        return NULL;
    }

    return GameAssetLibrary->at(idx);
}

// Get total amount of assets in library
unsigned int GameAssetManager::GetTotalGameAssets(void)
{
    // if asset library is null
    if(!GameAssetLibrary||GameAssetLibrary->size()==0)
    {
        return 0;
    }

    return GameAssetLibrary->size();
}

// wipe game asset from memory
bool GameAssetManager::DeleteGameAsset(GameAsset * RemoveGameAsset)
{
    // if asset library is null
    if(!GameAssetLibrary||GameAssetLibrary->size()==0)
    {
        return false;
    }

    // loop through library
    for(unsigned int i=0; i<GameAssetLibrary->size(); i++)
    {
        if(GameAssetLibrary->at(i)==RemoveGameAsset)
        {
            // Remove all Game Assets
            GameAssetLibrary->at(i)->RemoveClean();

            // remove from library
            GameAssetLibrary->erase(GameAssetLibrary->begin()+i);

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
    delete GameAssetLibrary;
    delete GameAssetRuleLibrary;

    // delete rest
    delete GameAssetResources;

    return;
}
