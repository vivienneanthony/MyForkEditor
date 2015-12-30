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

// Game Logic
#include "IGameLogic.h"

// Game View
#include "IGameView.h"

typedef Urho3D::List<Urho3D::SharedPtr<IGameView>> GameViewList;
typedef HashMap<GameNodeId, SharedPtr<Node>> GameNodesMap;

// UI
#include "IScreenElement.h"

typedef List<SharedPtr<IScreenElement>> ScreenElementList;

// Audio
#include "Audio/IAudio.h"
#include "Audio/IAudioBuffer.h"

typedef List<IAudioBuffer *> AudioBufferList;

// Input
#include "Controllers/IPointerHandler.h"
#include "Controllers/IKeyboardHandler.h"

// Network
#include "Network/IPacket.h"

// Identifier for our custom remote event 
typedef StringHash EventType;

// Events
#include "Events/IEventData.h"

// Scripting 
#include "Scripting/IScriptManager.h"

// Physics
#include "Physics/IGamePhysics.h"

// Chemistry
#include "Chemistry/IGameChemistry.h"

#endif
