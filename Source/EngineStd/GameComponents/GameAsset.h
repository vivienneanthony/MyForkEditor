#ifndef GAMEASSET_H
#define GAMEASSET_H

// include engine headers
#include "EngineStd.h"

#include "GameObject.h"

// interactive logic component
class GameAsset : public GameObject
{
    URHO3D_OBJECT(GameAsset, GameObject);

    public:
        GameAsset(Context * context);
        virtual ~GameAsset();
    protected:
    private:
};

#endif // INTERACTIVE_H
