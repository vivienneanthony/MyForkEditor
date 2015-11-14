#ifndef GameAssetManager_GameAssetData_Included
#define GameAssetManager_GameAssetData_Included

#include <vector>
#include <string>

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
    void SetGameAssetsPath(string addpath);
    void SetAddDataFile(string addfile);

    // serialize output
    void Serialize(string OutputDataFile, const vector<GameAsset*> & Data) { };

    // get resources
    vector<GameAsset*>* GetGameAssets(void);

private:
    // data path and data files
    string* m_pDataPath;
    vector<string>* m_pDataFiles;
};
#endif
