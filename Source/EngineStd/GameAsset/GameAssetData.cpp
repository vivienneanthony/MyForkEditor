#include "EngineStd.h"

#include <iostream>
#include <vector>

using namespace std;

#include "GameAssetData.h"

// constructor - initialize set default
GameAssetData::GameAssetData(Context *context)
    :Object(context),
    m_pDataPath(NULL),
	m_pDataFiles(NULL)
{

	m_pDataPath = new string();
	m_pDataFiles = new vector<string>();

}

// a resource path
void GameAssetData::SetGameAssetsPath(string addpath)
{
    if(!m_pDataPath)
    {
        return;
    }

    // empty and add path
    m_pDataPath->empty();
    m_pDataPath->append(addpath);

    return;
}

// add a resource file
void GameAssetData::SetAddDataFile(string addfile)
{
	if (!m_pDataFiles)
    {
        return;
    }

	m_pDataFiles->push_back(addfile);

    return;
}

// get resources from all files if need be
vector<GameAsset*>* GameAssetData::GetGameAssets(void)
{
    // temporary vector list
    vector<GameAsset*>* alldata = NULL;

    // if m_pDataPath of DataFile is not set exit in memory
	if (m_pDataPath == NULL || m_pDataFiles == NULL)
    {
        return NULL;
    }

    // if files and path not set
	if (m_pDataFiles->size() == 0 || m_pDataPath->empty())
    {
        return NULL;
    }

    return alldata;
}

// deconstructor
GameAssetData::~GameAssetData()
{
	SAFE_DELETE(m_pDataPath);
	SAFE_DELETE(m_pDataFiles);

}
