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

    return;
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

// Add resource file
void GameAssetData::SetAddDataFile(String addfile)
{
    if (!m_pDataFiles)
    {
        return;
    }

    m_pDataFiles->Push(addfile);

    return;
}

// Get resources - temporary
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

// Deserialize
bool GameAssetData::Deserialize(pugi::xml_node ThisChild, GameAsset * m_ThisChildGameAsset)
{
    // Get attributes from xml to flags to set
    const char * m_Name = ThisChild.attribute("Name").as_string();
    const char * m_Symbol = ThisChild.attribute("Symbol").as_string();
    unsigned int m_Type = ThisChild.attribute("Type").as_uint();
    unsigned int m_State = ThisChild.attribute("State").as_uint();
    float m_Density  = ThisChild.attribute("Density").as_float();
    float m_XPos = ThisChild.attribute("XPos").as_float();
    float m_YPos = ThisChild.attribute("YPos").as_float();
    float m_ZPos = ThisChild.attribute("ZPos").as_float();
    float m_Rotation = ThisChild.attribute("Rotation").as_float();
    unsigned int m_Quantity = ThisChild.attribute("Quantity").as_uint();
    bool m_IsPhysical = ThisChild.attribute("IsPhysical").as_bool();
    bool m_IsTradeable= ThisChild.attribute("IsTradeable").as_bool();
    bool m_IsPowered= ThisChild.attribute("IsPowered").as_bool();
    bool m_IsEntity = ThisChild.attribute("IsEntity").as_bool();
    const char * m_PhysicalModel = ThisChild.attribute("PhysicalModel").as_string();
    bool m_IsLinkedGameAsset = (bool)ThisChild.attribute("IsLinkedGameAsset").as_bool();
    bool m_UseAttachmentPoints = ThisChild.attribute("UseAttachmentPoints").as_bool();
    bool m_UseUpgradeSystem = ThisChild.attribute("UseUpgradeSystem").as_bool();
    bool m_UseRaritySystem = ThisChild.attribute("UseRaritySystem").as_bool();

    // Set attributes from xml to new game asset
    m_ThisChildGameAsset->SetName(String(m_Name));
    m_ThisChildGameAsset->SetSymbol(String(m_Symbol));
    m_ThisChildGameAsset->SetAttributes(m_IsPhysical, m_IsTradeable, m_IsPowered, m_IsEntity, m_IsLinkedGameAsset);
    m_ThisChildGameAsset->SetTypeState((GameAssetType)m_Type, (GameAssetState)m_State);
    m_ThisChildGameAsset->SetQuantity(m_Quantity);
    m_ThisChildGameAsset->SetPositionRotation(m_XPos, m_YPos, m_ZPos, m_Rotation);
    m_ThisChildGameAsset->SetSpecialAttributes(m_UseAttachmentPoints,m_UseUpgradeSystem,m_UseRaritySystem);
    m_ThisChildGameAsset->SetDensity(m_Density);
    m_ThisChildGameAsset->SetPhysicalModel(String(m_PhysicalModel));

    // create a new game asset
    GameAsset * m_GameAsset_NewChild = NULL;

    // For ... loop through each child
    for(pugi::xml_node ThisChild_Child = ThisChild.first_child(); ThisChild_Child; ThisChild_Child =  ThisChild_Child.next_sibling())
    {
        // Create a default asset
        m_GameAsset_NewChild = m_ThisChildGameAsset->AddChild("n/a","n/a",GAType_NotApplicable, GAState_NotApplicable);

        // Deserialize the next child
        Deserialize(ThisChild_Child, m_GameAsset_NewChild);
    }

    return true;
}

// Save all game assets
bool GameAssetData::SaveGameAssets(Vector<GameAsset *>* gameAssetData)
{
    // If GameAssetData is Null or size is empty exit
    if(gameAssetData == NULL || gameAssetData->Size() == 0)
    {
        return false;
    }

    // create xml document
    pugi::xml_document* doc = new pugi::xml_document();

    // create a child node
    pugi::xml_node root = doc->append_child();
    root.set_name("GameAssets");

    // loop through all game assets
    for(unsigned i = 0; i <= gameAssetData->Size(); ++i)
    {
        gameAssetData->At(i)->Serialize(root);
    }

    // write to file
    doc->save_file("GameAssetData.xml");

    // safe delete
    SAFE_DELETE(doc);

    return true;
}

// deconstructor
GameAssetData::~GameAssetData()
{
    SAFE_DELETE (m_pDataPath);
    SAFE_DELETE (m_pDataFiles);

    return;
}

// load game assets
bool GameAssetData::LoadGameAssets(Vector<GameAsset*> * gameAssetData)
{
    // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();
    FileSystem* filesystem = g_pApp->GetFileSystem();

    // Create a empty
    Vector<String> datafiles;

    // Use package file
    String usePackageFile;

    // Package found bool;
    bool bPackageFileFound = false;

    PackageFile * pPackageFile = new PackageFile(context_);

    // Setup base path
    String BasePath;
    BasePath.Append(filesystem->GetProgramDir());

    // Search files
    for(unsigned int i = 0; i < m_pDataFiles->Size(); i++)
    {
        // Clear Package Path
        String packagePathName;

        // Create temporary file path with resource
        packagePathName.Append(BasePath);
        packagePathName.Append(m_pDataFiles->At(i));

        // Open a packaged file
        bPackageFileFound = pPackageFile->Open(packagePathName);

        // If packaged file is found continue
        if(bPackageFileFound == true)
        {
            // Use this package if found and matches
            if(pPackageFile->Exists("GameAssets/GameAssetsData.xml"))
            {
                usePackageFile.Append(m_pDataFiles->At(i));
                bPackageFileFound = true;

                break;
            }
        }
    }

    // If datafiles is 0 or empty return - meaning nothing was found
    if(bPackageFileFound == false)
    {
        URHO3D_LOGERROR ("Game Asset Manager Data - Error Game Asset File Not Found");

        return false;
    }

    // Package Entry
    const PackageEntry* pGameAssetsDataEntry = pPackageFile->GetEntry("GameAssets/GameAssetsData.xml");

    // Set package
    File* pPackageData = new File(context_, pPackageFile, String("GameAssets/GameAssetsData.xml"));

    // Read content to memory
    unsigned char* retrievedsource = NULL;

    retrievedsource = new unsigned char[pGameAssetsDataEntry->size_];

    // Read data from file
    pPackageData->Read(&retrievedsource[0], pGameAssetsDataEntry->size_);

    // Create new xml document - Pugixml friendly
    pugi::xml_document doc;

    char* buffer = static_cast<char*>(pugi::get_memory_allocation_function()(pGameAssetsDataEntry->size_));
    memcpy(buffer, retrievedsource, pGameAssetsDataEntry->size_);

    pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, pGameAssetsDataEntry->size_);

    // Safe delete memory not in use
    delete [] retrievedsource;

    // Exit if error
    if (result.status != pugi::status_ok)
    {
        URHO3D_LOGERROR ("Game Asset Manager Data - Problem occured reading game assets");

        return false;
    }

    // Get XML root if it exist, if not exit
    pugi::xml_node GameAssetRoot = doc.first_child();

    if(!GameAssetRoot)
    {
        return false;
    }

    // For ... loop through each child
    for(pugi::xml_node NewGameAsset =  GameAssetRoot.first_child(); NewGameAsset; NewGameAsset =  NewGameAsset.next_sibling())
    {
        // create a game asset
        GameAsset * newGameAsset = new GameAsset(context_);
        gameAssetData->Push(newGameAsset);

        // Get attributes from xml
        const char* name = NewGameAsset.attribute("Name").as_string();
        const char* symbol = NewGameAsset.attribute("Symbol").as_string();
        unsigned int type = NewGameAsset.attribute("Type").as_uint();
        unsigned int state = NewGameAsset.attribute("State").as_uint();
        float density  = NewGameAsset.attribute("Density").as_float();
        float XPos = NewGameAsset.attribute("XPos").as_float();
        float YPos = NewGameAsset.attribute("YPos").as_float();
        float ZPos = NewGameAsset.attribute("ZPos").as_float();
        float rotation = NewGameAsset.attribute("Rotation").as_float();
        unsigned int quantity = NewGameAsset.attribute("Quantity").as_uint();
        bool isPhysical = NewGameAsset.attribute("IsPhysical").as_bool();
        bool isTradeable= NewGameAsset.attribute("IsTradeable").as_bool();
        bool isPowered= NewGameAsset.attribute("IsPowered").as_bool();
        bool isEntity = NewGameAsset.attribute("IsEntity").as_bool();
        const char* physicalModel =  NewGameAsset.attribute("PhysicalModel").as_string();
        bool isLinkedGameAsset = NewGameAsset.attribute("IsLinkedGameAsset").as_bool();
        bool useAttachmentPoints = NewGameAsset.attribute("UseAttachmentPoints").as_bool();
        bool useUpgradeSystem = NewGameAsset.attribute("UseUpgradeSystem").as_bool();
        bool useRaritySystem = NewGameAsset.attribute("UseRaritySystem").as_bool();

        // Set attributes from xml to new game asset
        newGameAsset->SetName(name);
        newGameAsset->SetSymbol(symbol);
        newGameAsset->SetAttributes(isPhysical, isTradeable, isPowered, isEntity, isLinkedGameAsset);
        newGameAsset->SetTypeState((GameAssetType)type, (GameAssetState)state);
        newGameAsset->SetQuantity(quantity);
        newGameAsset->SetPositionRotation(XPos, YPos, ZPos, rotation);
        newGameAsset->SetSpecialAttributes(useAttachmentPoints, useUpgradeSystem, useRaritySystem);
        newGameAsset->SetDensity(density);
        newGameAsset->SetPhysicalModel(String(physicalModel));

        // create a new game asset
        GameAsset* gameAsset_NewChild;

        // For ... loop through each child
        for (pugi::xml_node newGameAssetChild : NewGameAsset.children())
        {
            // Create a default child
            gameAsset_NewChild = newGameAsset->AddChild("n/a","n/a", GAType_NotApplicable, GAState_NotApplicable);
            Deserialize(newGameAssetChild, gameAsset_NewChild);
        }
    }

    // Safe delete
    SAFE_DELETE(pPackageData);
    SAFE_DELETE(pPackageFile);

    // Add addtional
    return true;
}

