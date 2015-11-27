#ifndef ENGINE_STD_H
#define ENGINE_STD_H

// Engine
#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineEvents.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/Engine/DebugHud.h>

// Instead of STL Containers
#include <Urho3D/Container/List.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/Hash.h>
#include <Urho3D/Container/Str.h>

// Core
#include <Urho3D/Core/Main.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/Core/Thread.h>
#include <Urho3D/Core/WorkQueue.h>
#include <Urho3D/Core/Mutex.h>

#include <Urho3D/Core/MiniDump.h>
#include <Urho3D/Core/Profiler.h>

#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Core/StringUtils.h>

// Graphics
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/GraphicsImpl.h>
#include <Urho3D/Graphics/GraphicsEvents.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderSurface.h>
#include <Urho3D/Graphics/RenderPath.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/BillboardSet.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Terrain.h>

// urho2d
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>

// Input/Output
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Input/Controls.h>

// Resource Cache
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>
// Scene
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/ReplicationState.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Animatable.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/Scene/UnknownComponent.h>
#include <Urho3D/Scene/LogicComponent.h>

// User Interface
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Sprite.h>

#include <Urho3D/UI/View3D.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/DropDownList.h>
#include <Urho3D/UI/ListView.h>
#include <Urho3D/UI/ScrollBar.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/UI/Menu.h>
#include <Urho3D/UI/Slider.h>
// Physics
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/PhysicsUtils.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>

// Animations
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/RenderSurface.h>

// Audio
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Audio/BufferedSoundStream.h>
#include <Urho3D/Audio/SoundStream.h>
#include <Urho3D/Audio/OggVorbisSoundStream.h>
#include <Urho3D/Audio/SoundListener.h>

// Math
#include "Graphics3D/Geometry.h"
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Vector4.h>
#include <Urho3D/Math/Random.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Math/AreaAllocator.h>


// Input/Output system
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/File.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/PackageFile.h>
#include <Urho3D/IO/Serializer.h>
#include <Urho3D/IO/MemoryBuffer.h>

#include <Urho3D/Network/Connection.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Network/HttpRequest.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/NetworkPriority.h>
#include <Urho3D/Network/Protocol.h>

#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptAPI.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/ScriptInstance.h>
#ifdef NDEBUG
	#undef URHO3D_LOGGING
#endif

// Debugging
//#include <Urho3D/DebugNew.h>

// Third parties
#include <Urho3D/ThirdParty/PugiXml/pugixml.hpp>

#if _MSC_VER >= 1600
	// Visual leak detector.
	#include <vld.h>
#endif


using namespace Urho3D;

// Some Engine utilities
#include "Utility/ApplicationMessage.h"
#include "Utility/Templates.h"

#include "UserInterface/Urho3D/Utilities.h"

#include "Debugging/Debugging.h"
#include "Interfaces/Interfaces.h"

#include "EngineApplication.h"

#include "EventManager/Events.h"

#include <exception>

#endif // ENGINE_STD_H
