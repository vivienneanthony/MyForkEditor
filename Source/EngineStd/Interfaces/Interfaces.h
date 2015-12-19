#ifndef INTERFACES_H
#define INTERFACES_H

class BaseComponent;

class MainComponent;
class GameNode;

typedef unsigned int GameNodeId;
typedef String GameNodeType;

typedef unsigned int ComponentId;

typedef SharedPtr<Node> StrongNodePtr;
typedef WeakPtr<Node> WeakNodePtr;

typedef SharedPtr<GameNode> StrongGameNodePtr;
typedef WeakPtr<GameNode> WeakGameNodePtr;

typedef SharedPtr<BaseComponent> StrongComponentPtr;
typedef WeakPtr<BaseComponent> WeakComponentPtr;

typedef SharedPtr<MainComponent> StrongGameNodeComponentPtr;
typedef WeakPtr<MainComponent> WeakGameNodeComponentPtr;


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

// Identifier for our custom remote event 
typedef StringHash EventType;

#include "Events/IEventData.h"

#endif
