#ifndef GameAssetManager_GameAsset_Included
#define GameAssetManager_GameAsset_Included

#include <vector>
#include <string>

using namespace std;

// Game Asset Type
enum GameAssetType
{
    /// Resource list
    GAType_None,
    GAType_PeriodicUnidentified,
    GAType_PeriodicElement,
    GAType_RawMaterial,
    GAType_Component,
    GAType_Tool,
    GAType_ForcefieldGenerator,
    GAType_Structural,
    GAType_Light,
    GAType_PowerSource,
    GAType_ReplicationPrinter,
    GAType_RefrigerationUnit,
    GAType_StorageContainer,
    GAType_DroneEntity,
    GAType_NPCEntity,
    GAType_PlayerEntity,
    GAType_NotApplicable=99999
};

// Game Asset Component States
enum GameAssetState
{
    GAState_None,
    GAState_PeriodicUnidentified,
    GAState_Gaseous,
    GAState_Liquid,
    GAState_Solid,
    GAState_NotApplicable=99999
};

class GameAsset : public Object
{
	URHO3D_OBJECT(GameAsset, Object)

public:
    // Constructor and Destructor
    GameAsset(Context * context);
    ~GameAsset();

    // set attributes
    void SetName(string setName);
    void SetSymbol(string setName);
    void SetTypeState(GameAssetType setType, GameAssetState setState);
    void SetAttributes(bool setPhysical, bool setPowered, bool setEntity, bool setLinkedGameAsset);

    // search asset by name
    GameAsset * FindChildByKeyword(string Keyword, bool useName);
    GameAsset * FindChildByName(string Keyword){return FindChildByKeyword(Keyword, true);};
    GameAsset * FindChildBySymbol(string Keyword){ return FindChildByKeyword(Keyword, false);};

    // add child
    GameAsset * AddChild(string GA_Name, string GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State);

    // remove children
    bool DeleteChild(GameAsset * RemoveGameAsset);
    void RemoveClean(void);

    // get name and symbol
    string GetName() {return Name;};
    string GetSymbol() {return Symbol;};

    void Dump(void);

private:
    // Name and Prefix
    string Name;
    string Symbol;

    // Element
    GameAssetType Type;
    GameAssetState State;

    // ResourceComponents
    vector <GameAsset *> * Children;

    // Desricptor
    float Density;

    // Flags
    bool IsPhysical;
    bool IsTradable;
    bool IsPowered;
    bool IsEntity;
    bool IsLinkedGameAsset;       // Should be tier 4

    /// Special Attributes = Default null
    string PhysicalModel;

};

class GameAssetRule
{
    public:
    GameAssetRule(){};
    ~GameAssetRule(){};

    private:
};

#endif
