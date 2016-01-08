#ifndef GameAssetManager_GameAssetRules_Included
#define GameAssetManager_GameAssetRules_Included

using namespace std;

class GameAssetRules : public Object
{
    URHO3D_OBJECT(GameAssetRules, Object);

public:
    // Constructor and Destructor
    GameAssetRules(Context * context);
    ~GameAssetRules();

private:
    // private data
};
#endif
