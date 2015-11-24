// Hangars
//

#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED

#include "EngineStd.h"

using namespace Urho3D;
using namespace std;

// Game Object base class
class GameObject : public LogicComponent
{
    URHO3D_OBJECT(GameObject, LogicComponent)

public:
    // Construct.
    GameObject(Context* context);
    virtual ~GameObject(void);

    // Register object factory and attributes.
    static void RegisterObject(Context* context);

    // Handle startup. Called by LogicComponent base class.
    virtual void Initialize();

    // Handle update. Called by LogicComponent base class.
    virtual void FixedUpdate(float timeStep);

    // return lifetime
    inline float GetLifetime(void)
    {
        return m_Lifetime;
    };

private:
    float m_Lifetime;       // lifetime
};

#endif // GAMEOBJECT_H_INCLUDED
