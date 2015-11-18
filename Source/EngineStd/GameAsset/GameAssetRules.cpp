#include "EngineStd.h"

#include <iostream>

using namespace std;

#include "GameAsset.h"
#include "GameAssetRules.h"

// constructor - initialize set default
GameAssetRules::GameAssetRules(Context *context)
:Object(context)
{
}

// deconstructor
GameAssetRules::~GameAssetRules()
{
    return;
}
