#ifndef GameAssetManager_GameAssetData_Included
#define GameAssetManager_GameAssetData_Included


using namespace std;
using namespace Urho3D;

#include "GameAsset.h"

class GameAssetData : public Object
{
    URHO3D_OBJECT(GameAssetData, Object);

public:
    // Constructor and Destructor
    GameAssetData(Context* context);
    ~GameAssetData();

    // set assets path
    void SetGameAssetsPath(String addpath);
    void SetAddDataFile(String addfile);

    // get resources
    Vector<GameAsset*>* GetGameAssets(void);

    // Save all game assets
    bool SaveGameAssets(Vector<GameAsset *>* gameAssetData);

    // load assets and get into memory
    bool Deserialize(pugi::xml_node ThisChild, GameAsset* thisChildGameAsset);
    bool LoadGameAssets(Vector<GameAsset*>* gameAssetData);

private:
    // data path and data files
    String* m_pDataPath;
    Vector<String>* m_pDataFiles;
};
#endif
