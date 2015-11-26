#ifndef GameAssetManager_GameAssetManager_Included
#define GameAssetManager_GameAssetManager_Included


#include "GameAssetData.h"

using namespace std;
using namespace Urho3D;

class URHO3D_API GameAssetManager : public LogicComponent
{
    URHO3D_OBJECT(GameAssetManager, LogicComponent);
public:
    // Constructor and Destructor
    GameAssetManager(Context* context);
    ~GameAssetManager();

    static void RegisterNewSubsystem(Context* context);

    // initialize
    void Init(void);

    // add game asset - base symbol
    GameAsset* AddGameAsset(String GA_Name, String GA_Symbol, GameAssetType GA_Type, GameAssetState GA_State);

    // search asset by name
    GameAsset* FindGameAssetByKeyword(String Keyword, bool useName);
    GameAsset* FindGameAssetByName(String Keyword){ return FindGameAssetByKeyword(Keyword, true); };
    GameAsset* FindGameAssetBySymbol(String Keyword){ return FindGameAssetByKeyword(Keyword, false); };

    // load initialial assets
    void InitializeBaseGameAssets(void);

    // load game assets
    bool LoadGameAssets(void)
    {
        if(m_pGameAssetLibrary == NULL || m_pGameAssetResources == NULL)
        {
            return NULL;
        }

        // load into memory
        return m_pGameAssetResources->LoadGameAssets(m_pGameAssetLibrary);
    };


    // find a specific asset
    GameAsset* GetGameAssetByIdx(unsigned int idx);

    // get total assets
    unsigned int GetTotalGameAssets(void);

    // delete asset
    bool DeleteGameAsset(GameAsset* RemoveGameAsset);

private:
    // Game Asset Library - Actual data
    Vector<GameAsset*>*		m_pGameAssetLibrary;
    Vector<GameAssetRule*>* m_pGameAssetRuleLibrary;

    GameAssetData*			m_pGameAssetResources;

};

#endif
