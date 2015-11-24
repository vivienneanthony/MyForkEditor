#ifndef GameAssetManager_GameAsset_Included
#define GameAssetManager_GameAsset_Included


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
	GAType_AmmoPickup,
	GAType_HealthPickup,
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
    GameAsset(Context* context);
    ~GameAsset();

    // set attributes
    void SetName(String setName);
    void SetSymbol(String setName);
    void SetTypeState(GameAssetType setType, GameAssetState setState);
    void SetAttributes(bool setPhysical, bool setTradeable, bool setPowered, bool setEntity, bool setLinkedGameAsset);
    void SetPositionRotation(float setXPos, float setYPos, float setZPos, float setRotation);
    void SetSpecialAttributes(bool setUseAttachments, bool setUseUpgradeSystem, bool setUseRaritySystem);

    void SetQuantity(unsigned int setQuantityAmount)
    {
        m_Quantity = setQuantityAmount;

        return;
    }

    void SetPhysicalModel(String setPhysicalModel)
    {
        m_PhysicalModel = setPhysicalModel;

        return;
    }

    void SetDensity(float setDensity)
    {
        m_Density = setDensity;

        return;
    }


    // search asset by name
    GameAsset* FindChildByKeyword(String Keyword, bool useName);
    GameAsset* FindChildByName(String Keyword){ return FindChildByKeyword(Keyword, true); };
    GameAsset* FindChildBySymbol(String Keyword){ return FindChildByKeyword(Keyword, false); };

    // add child
    GameAsset* AddChild(String GA_Name, String GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State);
	Vector<GameAsset*>& GetChilds() { return (*m_pChildrens); }
	
	// remove children
    bool DeleteChild(GameAsset* RemoveGameAsset);
    void RemoveClean(void);

    // Getters
	inline String GetName() { return m_Name; };
	inline String GetSymbol() { return m_Symbol; };
	inline GameAssetType GetType() { return m_Type; }

    void Dump(void);

    void Serialize(pugi::xml_node & ParentNode);

private:

    // Name and Prefix
    String m_Name;
    String m_Symbol;

    // Element
    GameAssetType m_Type;
    GameAssetState m_State;

    // ResourceComponents
    Vector <GameAsset *> * m_pChildrens;

    // Desricptor
    float m_Density;

    // Position
    float m_XPos;                         // X Xosition
    float m_YPos;                         // Y Position
    float m_ZPos;                         // Z Position
    float m_Rotation;                     // Rotation Quaternion

    // Quantity
    unsigned int m_Quantity;             // Use for linked assets

    // Flags
    bool m_bIsPhysical;
    bool m_bIsTradeable;
    bool m_bIsEntity;
    bool m_bIsPowered;
    bool m_bIsLinkedGameAsset;

    bool m_bUseAttachmentPoints;          // Future use not implemented
    bool m_bUseUpgradeSystem;             // Future use not implemented
    bool m_bUseRaritySystem;              // Future use not implemented

    /// Special Attributes = Default null
    String m_PhysicalModel;
};

class GameAssetRule
{
public:
    GameAssetRule(){};
    ~GameAssetRule(){};

private:
};

#endif
