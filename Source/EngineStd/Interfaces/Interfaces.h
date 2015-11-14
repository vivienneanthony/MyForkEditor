#ifndef INTERFACES_H
#define INTERFACES_H

class Actor;
class ActorComponent;

typedef unsigned int ActorId;

const ActorId INVALID_ACTOR_ID = 0;

#include "IGameLogic.h"

#include "IGameView.h"

typedef Urho3D::List<Urho3D::SharedPtr<IGameView>> GameViewList;
typedef HashMap<ActorId, SharedPtr<Node>> ActorsMap;

#include "IScreenElement.h"

typedef List<SharedPtr<IScreenElement>> ScreenElementList;

#include "Audio/IAudio.h"
#include "Audio/IAudioBuffer.h"

typedef List<IAudioBuffer *> AudioBufferList;

#include "Controllers/IPointerHandler.h"
#include "Controllers/IKeyboardHandler.h"

#include "Network/IPacket.h"

#endif
