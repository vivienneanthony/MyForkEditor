#include <iostream>
#include <vector>

using namespace std;

#include "EngineStd.h"

#include "GameAssetData.h"

// constructor - initialize set default
GameAssetData::GameAssetData(Context *context)
    :Object(context),
    DataPath(NULL),
    DataFiles(NULL)
{

    DataPath = new string();
    DataFiles = new vector<string>();

}

// a resource path
void GameAssetData::SetGameAssetsPath(string addpath)
{
    if(!DataPath)
    {
        return;
    }

    // empty and add path
    DataPath->empty();
    DataPath->append(addpath);

    return;
}

// add a resource file
void GameAssetData::SetAddDataFile(string addfile)
{
    if(!DataFiles)
    {
        return;
    }

    DataFiles->push_back(addfile);

    return;
}

// get resources from all files if need be
vector<GameAsset *> * GameAssetData::GetGameAssets(void)
{
    // temporary vector list
    vector<GameAsset *> * alldata = NULL;

    // if DataPath of DataFile is not set exit in memory
    if(DataPath == NULL || DataFiles == NULL)
    {
        return NULL;
    }

    // if files and path not set
    if(DataFiles->size()==0||DataPath->empty())
    {
        return NULL;
    }

    return alldata;
}

// deconstructor
GameAssetData::~GameAssetData()
{
    delete DataPath;
    delete DataFiles;

    return;
}
