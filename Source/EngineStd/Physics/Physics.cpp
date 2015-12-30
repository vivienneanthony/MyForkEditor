#include "EngineStd.h"

#include "Resources/XMLResourceLoader/XMLResourceLoader.h"

#include "NullPhysics.h"
#include "Physics.h"

EnginePhysics::EnginePhysics(Context* context) : IGamePhysics(context)
{

}

EnginePhysics::~EnginePhysics()
{

}

bool EnginePhysics::VInitialize()
{
	LoadXML();

	return true;
}

void EnginePhysics::LoadXML()
{  
	// Load the physics config file and grab the root XML node
	pugi::xml_document* document = XMLResourceLoader::LoadXMLDocument("GameNodes/Config/Physics.xml");
	pugi::xml_node root = document->first_child();
	assert(root);

	// Load all materials
	pugi::xml_node parentNode = root.child("PhysicsMaterials");
	assert(parentNode);

	for (pugi::xml_node node = parentNode.first_child(); node; node = node.next_sibling())
	{
		float restitution = 0;
		float friction = 0;
		restitution = node.attribute("restitution").as_float();
		friction = node.attribute("friction").as_float();
		m_MaterialTable.Insert(MakePair(String(node.value()), MaterialData((float)restitution, (float)friction)));
	}

	// Load all densities
	parentNode = root.child("DensityTable");
	assert(parentNode);

	for (pugi::xml_node node = parentNode.first_child(); node; node = node.next_sibling())
	{
		m_DensityTable.Insert(MakePair(String(node.value()), (float)atof(node.child_value())));
	}
}

void EnginePhysics::VSyncVisibleScene()
{

}

void EnginePhysics::VOnUpdate(float timeStep)
{

}

void EnginePhysics::VRenderDiagnostics()
{

}

float EnginePhysics::LookupSpecificGravity(const String& densityStr)
{
	float density = 0;
	auto densityIt = m_DensityTable.Find(densityStr);
	if (densityIt != m_DensityTable.End())
	{
		density = densityIt->second_;
	}
	else
	{
		URHO3D_LOGERROR("Failed to find density");
	}

	return density;
}

MaterialData EnginePhysics::LookupMaterialData(const String& materialStr)
{
	auto materialIt = m_MaterialTable.Find(materialStr);
	if (materialIt != m_MaterialTable.End())
	{
		return materialIt->second_;
	}
	else
	{
		URHO3D_LOGERROR("Failed to find material");
		return MaterialData(0, 0);
	}
}

Urho3D::SharedPtr<IGamePhysics> CreateGamePhysics()
{
	Urho3D::SharedPtr<IGamePhysics> gamePhysics;
	
	gamePhysics = new EnginePhysics(g_pApp->GetContext());
	if (gamePhysics.Get() && !gamePhysics->VInitialize())
	{
		// Physics failed to initialize. Delete it.
		gamePhysics.Reset();
	}

	return gamePhysics;
}

Urho3D::SharedPtr<IGamePhysics> CreateNullPhysics()
{
	Urho3D::SharedPtr<IGamePhysics> gamePhysics = Urho3D::SharedPtr<IGamePhysics>(new NullPhysics(g_pApp->GetContext()));
	if (gamePhysics.Get() && !gamePhysics->VInitialize())
	{
		// Physics failed to initialize. Delete it.
		gamePhysics.Reset();
	}

	return gamePhysics;
}