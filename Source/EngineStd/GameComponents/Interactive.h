#ifndef INTERACTIVE_H
#define INTERACTIVE_H

// include engine headers
#include "EngineStd.h"

#include "GameObject.h"

// interactive logic component
class Interactive : public GameObject
{
    URHO3D_OBJECT(Interactive, GameObject);

    public:
        Interactive(Context * context);
        virtual ~Interactive();
    protected:
    private:
};

#endif // INTERACTIVE_H
