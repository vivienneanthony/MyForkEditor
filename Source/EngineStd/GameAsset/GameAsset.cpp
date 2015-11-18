#include "EngineStd.h"

#include <iostream>

#include "GameAsset.h"

using namespace std;



// initialize set default
GameAsset::GameAsset(Context* context)
	:m_Type(GAType_None)
	,m_State(GAState_None)
	,m_pChildren(NULL)
	,m_Density(0.0f)
    ,m_bIsPhysical(false)
    ,m_bIsPowered(false)
    ,m_bIsEntity(false)
    ,m_bIsLinkedGameAsset(false)
    ,Object(context)
{
   // GameAssetLibrary
    m_pChildren = new Vector<GameAsset*>();

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
}

// set attributes
void GameAsset::SetAttributes(bool setPhysical, bool setPowered, bool setEntity, bool setLinkedGameAsset)
{
    /// set attributes
	m_bIsPhysical = setPhysical;
	m_bIsPowered = setPowered;
	m_bIsEntity = setEntity;
	m_bIsLinkedGameAsset = m_bIsLinkedGameAsset;

}

// output
void GameAsset::Dump(void)
{
    // out data
    cout << " " << m_Name.CString();
    cout << " " << m_Symbol.CString();
    cout << " "  << m_Type;
    cout << " "  << m_State;
    cout << " "  << m_Density;
    cout << " "  << m_bIsPhysical;
	cout << " "  << m_bIsPowered;
	cout << " "  << m_bIsEntity;
	cout << " "  << m_bIsLinkedGameAsset;

    // if no children
    if(m_pChildren == NULL)
    {
        cout << " " << "0";

        return;
    }

    // get number of children
    unsigned int numberChildren = m_pChildren->Size();

    cout << " " << numberChildren;

    // if children
    if(numberChildren>0)
    {
        for(unsigned int i =0; i < m_pChildren->Size();i++)
        {
            // start
            cout << " " << i << " " << "{";

            // serialize output
            m_pChildren->At(i)->Dump();

            // end
            cout << " }";
        }

    }
}
// add a child to a game asset
GameAsset* GameAsset::AddChild(String GA_Name, String GA_Symbol, GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!m_pChildren)
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

    // set symbol
    newGameAsset->SetSymbol(GA_Symbol);
    newGameAsset->SetTypeState(GA_Type,GA_State);

    // add to library
    m_pChildren->Push(newGameAsset);

    return newGameAsset;
}

// remove child

// wipe game asset from memory
bool GameAsset::DeleteChild(GameAsset* RemoveGameAsset)
{
    // if asset library is null
	if (!m_pChildren || m_pChildren->Size() == 0)
    {
        return false;
    }

    // loop through library
	for (unsigned int i = 0; i<m_pChildren->Size(); i++)
    {
		if (m_pChildren->At(i) == RemoveGameAsset)
        {
            // Remove all Game Assets
			m_pChildren->At(i)->RemoveClean();

            // remove from library
			m_pChildren->Erase(m_pChildren->Begin() + i);

            // remove from memory
            SAFE_DELETE(RemoveGameAsset);
        }
    }

    return true;
}

// find a child by keyword
GameAsset* GameAsset::FindChildByKeyword(String Keyword, bool useName=0)
{
	if (!m_pChildren)
    {
        return NULL;
    }

    // check valid
    if(Keyword.Empty())
    {
        return NULL;
    }

	for (unsigned int i = 0; m_pChildren->Size(); i++)
    {
		if (useName == true ? m_pChildren->At(i)->GetName() == Keyword : m_pChildren->At(i)->GetSymbol() == Keyword)
        {
			return m_pChildren->At(i);
        }
    }

    return NULL;
}


// remove asset
void GameAsset::RemoveClean(void)
{
    // if Children null
	if (!m_pChildren)
    {
        return;
    }

    /// Shutdown if children exist
	if (m_pChildren)
    {
        /// loop through each
		for (unsigned i = 0; i<m_pChildren->Size(); i++)
        {
			m_pChildren->At(i)->RemoveClean();
        }

		SAFE_DELETE(m_pChildren);
    }
}

// destructor
GameAsset::~GameAsset()
{
    // if null
	if (!m_pChildren)
    {
        return;
    }

    RemoveClean();

	SAFE_DELETE(m_pChildren);
}
