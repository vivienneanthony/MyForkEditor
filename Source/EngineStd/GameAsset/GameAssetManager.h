#ifndef GameAssetManager_GameAssetManager_Included
#define GameAssetManager_GameAssetManager_Included

#include <vector>
#include <string>

#include "GameAssetData.h"

using namespace std;
using namespace Urho3D;

class URHO3D_API GameAssetManager : public LogicComponent
{
    URHO3D_OBJECT(GameAssetManager, LogicComponent);

    public:
        // Constructor and Destructor
        GameAssetManager(Context * context);
        ~GameAssetManager();

        static void RegisterNewSubsystem(Context* context);

        // initialize
        void Init(void);

        // add game asset - base symbol
        GameAsset * AddGameAsset(string GA_Name, string GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State);

        // search asset by name
        GameAsset * FindGameAssetByKeyword(string Keyword, bool useName);
        GameAsset * FindGameAssetByName(string Keyword){return FindGameAssetByKeyword(Keyword, true);};
        GameAsset * FindGameAssetBySymbol(string Keyword){ return FindGameAssetByKeyword(Keyword, false);};

        // find a specific asset
        GameAsset * GetGameAssetByIdx(unsigned int idx);

        // get total assets
        unsigned int GetTotalGameAssets(void);

        // delete asset
        bool DeleteGameAsset(GameAsset * RemoveGameAsset);

    private:

        // Game Asset Library - Actual data
        vector<GameAsset *> * GameAssetLibrary;
        vector<GameAssetRule *> * GameAssetRuleLibrary;

        GameAssetData          * GameAssetResources;

};
#endif
