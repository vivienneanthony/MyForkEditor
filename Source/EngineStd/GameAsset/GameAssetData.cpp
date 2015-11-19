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
bool GameAssetData::Deserialize(pugi::xml_node & ThisChild, GameAsset * m_ThisChildGameAsset)
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
        Deserialize(ThisChild_Child,m_GameAsset_NewChild);
    }

    return true;
}

// Load Game Assets
bool GameAssetData::LoadGameAssets(Vector<GameAsset *> * m_GameAssetData)
{
    // Create new xml document
    pugi::xml_document doc;

    // Parse to see if information is loaded
    pugi::xml_parse_result result = doc.load_file("GameAssetData.xml");

    // Exit if error
    if (result.status!=pugi::status_ok)
    {
        return false;
    }

    // Get XML root if it exist, if not exit
    pugi::xml_node GameAssetRoot=doc.first_child();

    if(!GameAssetRoot)
    {
        return false;
    }

    // For ... loop through each child
    for(pugi::xml_node NewGameAsset =  GameAssetRoot.first_child(); NewGameAsset; NewGameAsset =  NewGameAsset.next_sibling())
    {
        // create a game asset
        GameAsset * m_NewGameAsset = new GameAsset(context_);
        m_GameAssetData->Push(m_NewGameAsset);

        // Get attributes from xml
        const char * m_Name = NewGameAsset.attribute("Name").as_string();
        const char * m_Symbol = NewGameAsset.attribute("Symbol").as_string();
        unsigned int m_Type = NewGameAsset.attribute("Type").as_uint();
        unsigned int m_State = NewGameAsset.attribute("State").as_uint();
        float m_Density  = NewGameAsset.attribute("Density").as_float();
        float m_XPos = NewGameAsset.attribute("XPos").as_float();
        float m_YPos = NewGameAsset.attribute("YPos").as_float();
        float m_ZPos = NewGameAsset.attribute("ZPos").as_float();
        float m_Rotation = NewGameAsset.attribute("Rotation").as_float();
        unsigned int m_Quantity = NewGameAsset.attribute("Quantity").as_uint();
        bool m_IsPhysical = NewGameAsset.attribute("IsPhysical").as_bool();
        bool m_IsTradeable= NewGameAsset.attribute("IsTradeable").as_bool();
        bool m_IsPowered= NewGameAsset.attribute("IsPowered").as_bool();
        bool m_IsEntity = NewGameAsset.attribute("IsEntity").as_bool();
        const char * m_PhysicalModel =  NewGameAsset.attribute("PhysicalModel").as_string();
        bool m_IsLinkedGameAsset = NewGameAsset.attribute("IsLinkedGameAsset").as_bool();
        bool m_UseAttachmentPoints = NewGameAsset.attribute("UseAttachmentPoints").as_bool();
        bool m_UseUpgradeSystem = NewGameAsset.attribute("UseUpgradeSystem").as_bool();
        bool m_UseRaritySystem = NewGameAsset.attribute("UseRaritySystem").as_bool();

        // Set attributes from xml to new game asset
        m_NewGameAsset->SetName(m_Name);
        m_NewGameAsset->SetSymbol(m_Symbol);
        m_NewGameAsset->SetAttributes(m_IsPhysical,  m_IsTradeable, m_IsPowered, m_IsEntity, m_IsLinkedGameAsset);
        m_NewGameAsset->SetTypeState((GameAssetType)m_Type, (GameAssetState)m_State);
        m_NewGameAsset->SetQuantity(m_Quantity);
        m_NewGameAsset->SetPositionRotation(m_XPos, m_YPos, m_ZPos, m_Rotation);
        m_NewGameAsset->SetSpecialAttributes(m_UseAttachmentPoints,m_UseUpgradeSystem,m_UseRaritySystem);
        m_NewGameAsset->SetDensity(m_Density);
        m_NewGameAsset->SetPhysicalModel(String(m_PhysicalModel));

        // create a new game asset
        GameAsset * m_GameAsset_NewChild;

        // For ... loop through each child
for (pugi::xml_node NewGameAssetChild: NewGameAsset.children())
        {
            // Create a default child
            m_GameAsset_NewChild = m_NewGameAsset->AddChild("n/a","n/a",GAType_NotApplicable,GAState_NotApplicable);

            Deserialize(NewGameAssetChild,m_GameAsset_NewChild);
        }
    }

    return true;
}

// Save all game assets
bool GameAssetData::SaveGameAssets(Vector<GameAsset *> * m_GameAssetData)
{
    // If GameAssetData is Null or size is empty exit
    if(m_GameAssetData==NULL||m_GameAssetData->Size()==0)
    {
        return false;
    }

    // create xml document
    pugi::xml_document * doc;

    doc = new pugi::xml_document ();

    // create a child node
    pugi::xml_node Root = doc->append_child();
    Root.set_name("GameAssets");

    // loop through all game assets
    for(unsigned i=0; i<=m_GameAssetData->Size(); ++i)
    {
        m_GameAssetData->At(i)->Serialize(Root);
    }

    // write to file
    doc->save_file("GameAssetData.xml");

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
bool GameAssetData::LoadNewGameAssets(Vector<GameAsset*> * m_GameAssetData)
{
    // Get Resource
    ResourceCache* resCache = g_pApp->GetConstantResCache();
    FileSystem * filesystem = GetSubsystem<FileSystem>();

    // Create a empty
    Vector<String> m_datafiles;

    // Use package file
    String m_UsePackageFile;

    // Package found bool;
    bool m_bPackageFileFound=false;

    // create a package file

    PackageFile * m_pPackageFile = new PackageFile(context_);

    // base path
    String BasePath;
    BasePath.Append(filesystem->GetProgramDir());

    // search files
    for(unsigned i=0; i<=m_pDataFiles->Size(); ++i)
    {
        // clear
        String PackagePathName;

        PackagePathName.Append(BasePath);

        PackagePathName.Append(m_pDataPath->CString());

        // get current platform
        if(g_pApp->GetCurrentPlatform() != PlatformWindows)
        {
            PackagePathName.Append("/");
        }
        else
        {
            PackagePathName.Append("\\");
        }

        PackagePathName.Append(m_pDataFiles->At(i));

        // open package file
        m_bPackageFileFound = m_pPackageFile->Open(PackagePathName);

        // if file is found
        if(m_bPackageFileFound!=false&&m_bPackageFileFound?m_pPackageFile->Exists(String("GameAssetOptions.xml")):false)
        {
            // Use this package
            m_UsePackageFile.Append(m_pDataFiles->At(i));
            m_bPackageFileFound=true;

            break;
        }
    }

    // If datafiles is 0 or empty return - meaning nothing was found
    if(m_bPackageFileFound==false)
    {
        return false;
    }

    // Package Entry
    const PackageEntry * m_GameAssetsDataEntry= m_pPackageFile->GetEntry(String("GameAssetsOptions.xml"));

    // set package
    File * m_PackageData =  new File(context_, m_pPackageFile, String("GameAssetsOptions.xml"));

    // read into memory
    unsigned int* buffer = (unsigned int*) new char[m_GameAssetsDataEntry->size_];

    m_PackageData->Read (buffer, m_GameAssetsDataEntry->size_);

    // Create new xml document
    pugi::xml_document doc;

    // Parse to see if information is loaded
    pugi::xml_parse_result result = doc.load_buffer(buffer, sizeof(buffer));

    // Exit if error
    if (result.status!=pugi::status_ok)
    {
        return false;
    }

    // Get XML root if it exist, if not exit
    pugi::xml_node GameAssetRoot=doc.first_child();

    if(!GameAssetRoot)
    {
        return false;
    }

    // For ... loop through each child
    for(pugi::xml_node NewGameAsset =  GameAssetRoot.first_child(); NewGameAsset; NewGameAsset =  NewGameAsset.next_sibling())
    {
        // create a game asset
        GameAsset * m_NewGameAsset = new GameAsset(context_);
        m_GameAssetData->Push(m_NewGameAsset);

        // Get attributes from xml
        const char * m_Name = NewGameAsset.attribute("Name").as_string();
        const char * m_Symbol = NewGameAsset.attribute("Symbol").as_string();
        unsigned int m_Type = NewGameAsset.attribute("Type").as_uint();
        unsigned int m_State = NewGameAsset.attribute("State").as_uint();
        float m_Density  = NewGameAsset.attribute("Density").as_float();
        float m_XPos = NewGameAsset.attribute("XPos").as_float();
        float m_YPos = NewGameAsset.attribute("YPos").as_float();
        float m_ZPos = NewGameAsset.attribute("ZPos").as_float();
        float m_Rotation = NewGameAsset.attribute("Rotation").as_float();
        unsigned int m_Quantity = NewGameAsset.attribute("Quantity").as_uint();
        bool m_IsPhysical = NewGameAsset.attribute("IsPhysical").as_bool();
        bool m_IsTradeable= NewGameAsset.attribute("IsTradeable").as_bool();
        bool m_IsPowered= NewGameAsset.attribute("IsPowered").as_bool();
        bool m_IsEntity = NewGameAsset.attribute("IsEntity").as_bool();
        const char * m_PhysicalModel =  NewGameAsset.attribute("PhysicalModel").as_string();
        bool m_IsLinkedGameAsset = NewGameAsset.attribute("IsLinkedGameAsset").as_bool();
        bool m_UseAttachmentPoints = NewGameAsset.attribute("UseAttachmentPoints").as_bool();
        bool m_UseUpgradeSystem = NewGameAsset.attribute("UseUpgradeSystem").as_bool();
        bool m_UseRaritySystem = NewGameAsset.attribute("UseRaritySystem").as_bool();

        // Set attributes from xml to new game asset
        m_NewGameAsset->SetName(m_Name);
        m_NewGameAsset->SetSymbol(m_Symbol);
        m_NewGameAsset->SetAttributes(m_IsPhysical,  m_IsTradeable, m_IsPowered, m_IsEntity, m_IsLinkedGameAsset);
        m_NewGameAsset->SetTypeState((GameAssetType)m_Type, (GameAssetState)m_State);
        m_NewGameAsset->SetQuantity(m_Quantity);
        m_NewGameAsset->SetPositionRotation(m_XPos, m_YPos, m_ZPos, m_Rotation);
        m_NewGameAsset->SetSpecialAttributes(m_UseAttachmentPoints,m_UseUpgradeSystem,m_UseRaritySystem);
        m_NewGameAsset->SetDensity(m_Density);
        m_NewGameAsset->SetPhysicalModel(String(m_PhysicalModel));

        // create a new game asset
        GameAsset * m_GameAsset_NewChild;

        // For ... loop through each child
for (pugi::xml_node NewGameAssetChild: NewGameAsset.children())
        {
            // Create a default child
            m_GameAsset_NewChild = m_NewGameAsset->AddChild("n/a","n/a",GAType_NotApplicable,GAState_NotApplicable);

            Deserialize(NewGameAssetChild,m_GameAsset_NewChild);
        }
    }

    // Safe delete
    SAFE_DELETE(buffer);
    SAFE_DELETE(m_PackageData);
    SAFE_DELETE(m_pPackageFile);

    return true;
}
