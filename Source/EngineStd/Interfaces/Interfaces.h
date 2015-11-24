#ifndef INTERFACES_H
#define INTERFACES_H

class BaseComponent;

typedef unsigned int GameNodeId;
typedef unsigned int ComponentId;

typedef SharedPtr<Node> StrongNodePtr;
typedef WeakPtr<Node> WeakNodePtr;
typedef SharedPtr<BaseComponent> StrongComponentPtr;
typedef WeakPtr<BaseComponent> WeakComponentPtr;

const GameNodeId INVALID_GAME_NODE_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

#include "IGameLogic.h"

#include "IGameView.h"

typedef Urho3D::List<Urho3D::SharedPtr<IGameView>> GameViewList;
typedef HashMap<GameNodeId, SharedPtr<Node>> GameNodesMap;

#include "IScreenElement.h"

typedef List<SharedPtr<IScreenElement>> ScreenElementList;

#include "Audio/IAudio.h"
#include "Audio/IAudioBuffer.h"

typedef List<IAudioBuffer *> AudioBufferList;

#include "Controllers/IPointerHandler.h"
#include "Controllers/IKeyboardHandler.h"

#include "Network/IPacket.h"

#endif
