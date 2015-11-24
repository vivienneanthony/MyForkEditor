// include engine headers
#include "EngineStd.h"

// include header
#include "GameObject.h"

// initialize
GameObject::GameObject(Context *context)
    :LogicComponent(context)
    ,m_Lifetime(0.0f)
{
    // Set lifetime to 0
    m_Lifetime=0.0f;

    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);

    return;
}


// Initialization
void GameObject::Initialize()
{
    return;
}

// Register
void GameObject::RegisterObject(Context* context)
{
    return;

}

// Fixed update
void GameObject::FixedUpdate(float timeStep)
{
    m_Lifetime+=timeStep;

    return;
}


// Destructor
GameObject::~GameObject(void)
{
    return;
}


