#include <iostream>
#include <vector>

#include "EngineStd.h"

using namespace std;

#include "GameAsset.h"

// initialize set default
GameAsset::GameAsset(Context * context)
    :Type(GAType_None)
    ,State(GAState_None)
    ,Children(NULL)
    ,Density(0.0f)
    ,IsPhysical(false)
    ,IsPowered(false)
    ,IsEntity(false)
    ,IsLinkedGameAsset(false)
    ,Object(context)
{
   // GameAssetLibrary
    Children = new vector<GameAsset *>();

    return;
}

// set name
void GameAsset::SetName(string setName)
{
    Name=setName;
}

// set name
void GameAsset::SetSymbol(string setName)
{
    Symbol=setName;
}

// set state
void GameAsset::SetTypeState(GameAssetType setType, GameAssetState setState)
{
    Type=setType;
    State=setState;
}

// set attributes
void GameAsset::SetAttributes(bool setPhysical, bool setPowered, bool setEntity, bool setLinkedGameAsset)
{
    /// set attributes
    IsPhysical=setPhysical;
    IsPowered=setPowered;
    IsEntity=setEntity;
    IsLinkedGameAsset=IsLinkedGameAsset;

}

// output
void GameAsset::Dump(void)
{
    // out data
    cout << " " << Name.c_str();
    cout << " " << Symbol.c_str();
    cout << " "  << Type;
    cout << " "  << State;
    cout << " "  << Density;
    cout << " "  << IsPhysical;
    cout << " "  << IsPowered;
    cout << " "  << IsEntity;
    cout << " "  << IsLinkedGameAsset;

    // if no children
    if(Children==NULL)
    {
        cout << " " << "0";

        return;
    }

    // get number of children
    unsigned int numberChildren = Children->size();

    cout << " " << numberChildren;

    // if children
    if(numberChildren>0)
    {
        for(unsigned int i =0; i<Children->size();i++)
        {
            // start
            cout << " " << i << " " << "{";

            // serialize output
            Children->at(i)->Dump();

            // end
            cout << " }";
        }

    }
}
// add a child to a game asset
GameAsset * GameAsset::AddChild(string GA_Name, string GA_Symbol,GameAssetType GA_Type, GameAssetState GA_State)
{
    // if asset library is null
    if(!Children)
    {
        return NULL;
    }

    // check valid
    if(GA_Name.empty()||GA_Symbol.empty())
    {
        return NULL;
    }

    // check if state or type is valid
    if(GA_Type==GAType_None||GA_State==GAState_None)
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
    Children->push_back(newGameAsset);

    return newGameAsset;
}

// remove child

// wipe game asset from memory
bool GameAsset::DeleteChild(GameAsset * RemoveGameAsset)
{
    // if asset library is null
    if(!Children||Children->size()==0)
    {
        return false;
    }

    // loop through library
    for(unsigned int i=0; i<Children->size(); i++)
    {
        if(Children->at(i)==RemoveGameAsset)
        {
            // Remove all Game Assets
            Children->at(i)->RemoveClean();

            // remove from library
            Children->erase(Children->begin()+i);

            // remove from memory
            delete RemoveGameAsset;
        }
    }

    return true;
}

// find a child by keyword
GameAsset * GameAsset::FindChildByKeyword(string Keyword, bool useName=0)
{
    if(!Children)
    {
        return NULL;
    }

    // check valid
    if(Keyword.empty())
    {
        return NULL;
    }

    for(unsigned int i=0; Children->size(); i++)
    {
        if(useName==true? Children->at(i)->GetName()== Keyword : Children->at(i)->GetSymbol()==Keyword)
        {
            return Children->at(i);
        }
    }


    return NULL;
}


// remove asset
void GameAsset::RemoveClean(void)
{
    // if Children null
    if(!Children)
    {
        return;
    }

    /// Shutdown if children exist
    if(Children)
    {
        /// loop through each
        for (unsigned i=0; i<Children->size(); i++)
        {
            Children->at(i)->RemoveClean();
        }

        delete Children;
    }
}

// destructor
GameAsset::~GameAsset()
{
    // if null
    if(!Children)
    {
        return;
    }

    RemoveClean();

    delete Children;

    return;
}
