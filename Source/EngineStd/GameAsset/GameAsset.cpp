#include "EngineStd.h"

#include <iostream>

#include "GameAsset.h"

using namespace std;



// initialize set default
GameAsset::GameAsset(Context* context)
   :m_Type(GAType_None)
    ,m_State(GAState_None)
    ,m_pChildrens(NULL)
    ,m_Density(0.0f)
    ,m_XPos(0.0f)
    ,m_YPos(0.0f)
    ,m_ZPos(0.0f)
    ,m_Rotation(0.0f)
    ,m_Quantity(1)
    ,m_bIsPhysical(false)
    ,m_bIsPowered(false)
    ,m_bIsTradeable(false)
    ,m_bIsEntity(false)
    ,m_bIsLinkedGameAsset(false)
    ,m_bUseAttachmentPoints(false)
    ,m_bUseUpgradeSystem(false)
    ,m_bUseRaritySystem(false)
    ,Object(context)
{
   // GameAssetLibrary
    m_pChildrens = new Vector<GameAsset*>();

    return;
}

// set name
void GameAsset::SetName(String setName)
{
    m_Name = setName;
}

// set name
void GameAsset::SetSymbol(String setName)
{
	m_Symbol = setName;
}

// set state
void GameAsset::SetTypeState(GameAssetType setType, GameAssetState setState)
{
	m_Type = setType;
	m_State = setState;

	return;
}

// set attributes
void GameAsset::SetAttributes(bool setPhysical, bool setTradeable, bool setPowered, bool setEntity, bool setLinkedGameAsset)
{
    /// set attributes
	m_bIsPhysical = setPhysical;
	m_bIsPowered = setPowered;
	m_bIsEntity = setEntity;
	m_bIsTradeable = setTradeable;
	m_bIsLinkedGameAsset = m_bIsLinkedGameAsset;

	return;
}

// Set Rotation and Position
void GameAsset::SetPositionRotation(float setXPos, float setYPos, float setZPos, float setRotation)
{
    /// set coordinate
    m_XPos = setXPos;
    m_YPos = setYPos;
    m_ZPos = setZPos;
    m_Rotation = setRotation;

    return;
}

// special flags
void GameAsset::SetSpecialAttributes(bool setUseAttachments, bool setUseUpgradeSystem, bool setUseRaritySystem)
{
    m_bUseUpgradeSystem = setUseUpgradeSystem;
    m_bUseAttachmentPoints = setUseAttachments;
    m_bUseRaritySystem = setUseRaritySystem;

    return;
}

void GameAsset::Serialize(pugi::xml_node & ParentNode)
// output
{
    // create a asset child
    pugi::xml_node ThisChild= ParentNode.append_child("Asset");

    /// if its not a linked asset show full details
    if(m_bIsLinkedGameAsset==false)
    {
        // Dump Info
        ThisChild.append_attribute("Name") = m_Name.CString();
        ThisChild.append_attribute("Symbol") = m_Symbol.CString();
        ThisChild.append_attribute("Type") = (unsigned int)m_Type;
        ThisChild.append_attribute("State") = (unsigned int)m_State;
        ThisChild.append_attribute("Density") = (float)m_Density;
        ThisChild.append_attribute("IsPhysical") = (bool)m_bIsPhysical;
        ThisChild.append_attribute("IsTradable") = (bool)m_bIsTradeable;
        ThisChild.append_attribute("IsPowered") = (bool)m_bIsPowered;
        ThisChild.append_attribute("IsEntity") = (bool)m_bIsEntity;
        ThisChild.append_attribute("IsLinkedGameAsset") = (bool)m_bIsLinkedGameAsset;
        ThisChild.append_attribute("UseAttachmentPoints") = (bool)m_bUseAttachmentPoints;
        ThisChild.append_attribute("UseUpgradeSystem") = (bool)m_bUseUpgradeSystem;
        ThisChild.append_attribute("UseRaritySystem") = (bool)m_bUseRaritySystem;
    }
    else
    {
        ThisChild.append_attribute("Symbol") = m_Symbol.CString();
        ThisChild.append_attribute("XPos") = (float)m_XPos;
        ThisChild.append_attribute("YPos") = (float)m_YPos;
        ThisChild.append_attribute("ZPos") = (float)m_ZPos;
        ThisChild.append_attribute("Quantity") = (unsigned int)m_Quantity;
        ThisChild.append_attribute("Rotation") = (float)m_Rotation;
        ThisChild.append_attribute("IsLinkedGameAsset") = (bool)m_bIsLinkedGameAsset;
    }

    // If no m_pChildren return
    if(m_pChildrens == NULL)
    {
        return;
    }

    // get number of m_pChildren
    unsigned int numberChildren = m_pChildrens->Size();

    // if m_pChildren
    if(numberChildren>0)
    {
        for(unsigned int i =0; i<m_pChildrens->Size(); i++)
        {
            // serialize output
            m_pChildrens->At(i)->Serialize(ThisChild);
        }
    }

    return;
}


// output
void GameAsset::Dump(void)
{
     //// if its not a linked asset show full details
    if(m_bIsLinkedGameAsset==false)
    {
        // out data
        cout << " " << m_Name.CString();
        cout << " " << m_Symbol.CString();
        cout << " "  << m_Type;
        cout << " "  << m_State;
        cout << " "  << m_Density;
        cout << " "  << m_bIsPhysical;
        cout << " "  << m_bIsTradeable;
        cout << " "  << m_bIsPowered;
        cout << " "  << m_bIsEntity;
        cout << " "  << m_bIsLinkedGameAsset;
        cout << " "  << m_bUseAttachmentPoints;
        cout << " "  << m_bUseUpgradeSystem;
        cout << " "  << m_bUseRaritySystem;
    }
    else
    {
        cout << " " << m_Symbol.CString();
        cout << " "  << m_XPos;
        cout << " "  << m_YPos;
        cout << " "  << m_ZPos;
        cout << " "  << m_Rotation;
        cout << " "  << m_Quantity;
        cout << " "  << m_bIsLinkedGameAsset;
    }

    // if no children
    if(m_pChildrens == NULL)
    {
        cout << " " << "0";

        return;
    }

    // get number of children
    unsigned int numberChildren = m_pChildrens->Size();

    cout << " " << numberChildren;

    // if children
    if(numberChildren>0)
    {
        for(unsigned int i =0; i < m_pChildrens->Size();i++)
        {
            // start
            cout << " " << i << " " << "{";

            // serialize output
            m_pChildrens->At(i)->Dump();

            // end
            cout << " }";
        }

    }
}
// add a child to a game asset
GameAsset* GameAsset::AddChild(String GA_Name, String GA_Symbol, GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!m_pChildrens)
    {
        return NULL;
    }

    // check valid
    if(GA_Name.Empty() || GA_Symbol.Empty())
    {
        return NULL;
    }

    // check if state or type is valid
    if(GA_Type == GAType_None || GA_State == GAState_None)
    {
        return NULL;
    }

    // create new game asset
    GameAsset * newGameAsset;

    newGameAsset = new GameAsset(context_);

    // Set symbol name
    newGameAsset->SetName("n/a");
    newGameAsset->SetSymbol(GA_Symbol);
    newGameAsset->SetTypeState(GA_Type,GA_State);

    // add to library
    m_pChildrens->Push(newGameAsset);

    return newGameAsset;
}

// wipe game asset from memory
bool GameAsset::DeleteChild(GameAsset* RemoveGameAsset)
{
    // if asset library is null
	if (!m_pChildrens || m_pChildrens->Size() == 0)
    {
        return false;
    }

    // loop through library
	for (unsigned int i = 0; i < m_pChildrens->Size(); i++)
    {
		if (m_pChildrens->At(i) == RemoveGameAsset)
        {
            // Remove all Game Assets
			m_pChildrens->At(i)->RemoveClean();

            // remove from library
			m_pChildrens->Erase(m_pChildrens->Begin() + i);

            // remove from memory
            SAFE_DELETE(RemoveGameAsset);
        }
    }

    return true;
}

// find a child by keyword
GameAsset* GameAsset::FindChildByKeyword(String Keyword, bool useName=0)
{
	if (!m_pChildrens)
    {
        return NULL;
    }

    // check valid
    if(Keyword.Empty())
    {
        return NULL;
    }

	for (unsigned int i = 0; m_pChildrens->Size(); i++)
    {
		if (useName == true ? m_pChildrens->At(i)->GetName() == Keyword : m_pChildrens->At(i)->GetSymbol() == Keyword)
        {
			return m_pChildrens->At(i);
        }
    }

    return NULL;
}


// remove asset
void GameAsset::RemoveClean(void)
{
    // if Children null
	if (!m_pChildrens)
    {
        return;
    }

    /// Shutdown if children exist
	if (m_pChildrens)
    {
        /// loop through each
		Urho3D::Vector<GameAsset*>::Iterator it = m_pChildrens->Begin();
		while (it != m_pChildrens->End())
        {
			// Remove childrens
			(*it)->RemoveClean();
			
			// Clean memory 
			SAFE_DELETE(*it);

			// Erase an element by iterator. Return iterator to the next element.
			it = m_pChildrens->Erase(it);
        }
    }
}

// destructor
GameAsset::~GameAsset()
{
    // if null
	if (!m_pChildrens)
    {
        return;
    }

    // Remove clean
    RemoveClean();

    // Clean pointer on vector
	SAFE_DELETE(m_pChildrens);
}
