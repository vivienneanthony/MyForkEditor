#include "EngineStd.h"

#include <iostream>
#include <vector>
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
    m_pChildren = new vector<GameAsset*>();

    return;
}

// set name
void GameAsset::SetName(string setName)
{
    m_Name = setName;
}

// set name
void GameAsset::SetSymbol(string setName)
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
    cout << " " << m_Name.c_str();
    cout << " " << m_Symbol.c_str();
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
    unsigned int numberChildren = m_pChildren->size();

    cout << " " << numberChildren;

    // if children
    if(numberChildren>0)
    {
        for(unsigned int i =0; i < m_pChildren->size();i++)
        {
            // start
            cout << " " << i << " " << "{";

            // serialize output
            m_pChildren->at(i)->Dump();

            // end
            cout << " }";
        }

    }
}
// add a child to a game asset
GameAsset* GameAsset::AddChild(string GA_Name, string GA_Symbol, GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!m_pChildren)
    {
        return NULL;
    }

    // check valid
    if(GA_Name.empty() || GA_Symbol.empty())
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
    m_pChildren->push_back(newGameAsset);

    return newGameAsset;
}

// remove child

// wipe game asset from memory
bool GameAsset::DeleteChild(GameAsset* RemoveGameAsset)
{
    // if asset library is null
	if (!m_pChildren || m_pChildren->size() == 0)
    {
        return false;
    }

    // loop through library
	for (unsigned int i = 0; i<m_pChildren->size(); i++)
    {
		if (m_pChildren->at(i) == RemoveGameAsset)
        {
            // Remove all Game Assets
			m_pChildren->at(i)->RemoveClean();

            // remove from library
			m_pChildren->erase(m_pChildren->begin() + i);

            // remove from memory
            SAFE_DELETE(RemoveGameAsset);
        }
    }

    return true;
}

// find a child by keyword
GameAsset* GameAsset::FindChildByKeyword(string Keyword, bool useName=0)
{
	if (!m_pChildren)
    {
        return NULL;
    }

    // check valid
    if(Keyword.empty())
    {
        return NULL;
    }

	for (unsigned int i = 0; m_pChildren->size(); i++)
    {
		if (useName == true ? m_pChildren->at(i)->GetName() == Keyword : m_pChildren->at(i)->GetSymbol() == Keyword)
        {
			return m_pChildren->at(i);
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
		for (unsigned i = 0; i<m_pChildren->size(); i++)
        {
			m_pChildren->at(i)->RemoveClean();
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
