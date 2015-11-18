#include "EngineStd.h"
#include "Resources/ResourceCache.h"

#include <iostream>


using namespace std;

#include "GameAssetData.h"

// constructor - initialize set default
GameAssetData::GameAssetData(Context *context)
    :Object(context),
    m_pDataPath(NULL),
    m_pDataFiles(NULL)
{

    m_pDataPath = new String();
    m_pDataFiles = new Vector<String>();

}

// a resource path
void GameAssetData::SetGameAssetsPath(String addpath)
{
    if(!m_pDataPath)
    {
        return;
    }

    // empty and add path
    m_pDataPath->Empty();
    m_pDataPath->Append(addpath);

    return;
}

// add a resource file
void GameAssetData::SetAddDataFile(String addfile)
{
    if (!m_pDataFiles)
    {
        return;
    }

    m_pDataFiles->Push(addfile);

    return;
}

// get resources from all files if need be
Vector<GameAsset*>* GameAssetData::GetGameAssets(void)
{
    // temporary Vector list
    Vector<GameAsset*>* alldata = NULL;

    // if m_pDataPath of DataFile is not set exit in memory
    if (m_pDataPath == NULL || m_pDataFiles == NULL)
    {
        return NULL;
    }

    // if files and path not set
    if (m_pDataFiles->Size() == 0 || m_pDataPath->Empty())
    {
        return NULL;
    }

    return alldata;
}

// load game assets
bool GameAssetData::LoadGameAssets(Vector<GameAsset*>* AllData)
{
    // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();

    // create a empty file
    Vector<String> m_datafiles;

    // search files
    for(unsigned i=0; i<=m_pDataFiles->Size(); ++i)
    {
        // temporary clear data
        m_datafiles.Clear();

        m_datafiles = SWResourceCache::Match(g_pApp->GetConstantResCache(), m_pDataFiles->At(i), m_pDataPath->CString());

        // if file is found
        if(m_datafiles.Size()>0)
        {
            // awesome the pak was found and a list of files is shown

            break;
        }
    }

    // if datafiles is 0 or empty return - meaning nothing was found
    if(m_datafiles.Size()==0||m_datafiles.Empty())
    {
        return false;
    }

    //  Find matching file flag
    bool m_bfounddatafile = false;

    // Scan for matching file
    for(unsigned int i=0; i<m_datafiles.Size(); i++)
    {
        if(m_datafiles.At(i).Find(String("GameAssetsData.xml",false)))
        {
            m_bfounddatafile = true;

            break;
        }
    }

    return true;
}

// deconstructor
GameAssetData::~GameAssetData()
{
    SAFE_DELETE(m_pDataPath);
    SAFE_DELETE(m_pDataFiles);

}
