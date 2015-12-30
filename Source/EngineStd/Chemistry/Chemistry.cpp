#include "EngineStd.h"

#include "Resources/XMLResourceLoader/XMLResourceLoader.h"

#include "NullChemistry.h"
#include "Chemistry.h"

EngineChemistry::EngineChemistry(Context* context) : IGameChemistry(context)
{

}

EngineChemistry::~EngineChemistry()
{

}

bool EngineChemistry::VInitialize()
{
	LoadXML();

	return true;
}

void EngineChemistry::LoadXML()
{
	// Load the physics config file and grab the root XML node
	pugi::xml_document* document = XMLResourceLoader::LoadXMLDocument("GameNodes/Config/Chemistry.xml");
	pugi::xml_node root = document->first_child();
	assert(root);

	// Load all raw materials
	pugi::xml_node parentNode = root.child("RawMaterialTable");
	assert(parentNode);

	for (pugi::xml_node node = parentNode.first_child(); node; node = node.next_sibling())
	{
		String name = node.attribute("Name").as_string();
		String symbol = node.attribute("Symbol").as_string();
		float density = node.attribute("Density").as_float();
		m_RawMaterialTable.Insert(MakePair(String(node.value()), RawMaterialData(name, symbol, density)));
	}
}

void EngineChemistry::VOnUpdate(float timeStep)
{

}

RawMaterialData EngineChemistry::LookupRawMaterialInfo(const String& rawMaterialStr)
{
	auto rawMaterialIt = m_RawMaterialTable.Find(rawMaterialStr);
	if (rawMaterialIt == m_RawMaterialTable.End())
	{
		URHO3D_LOGERROR("Failed to find raw material data");
		return RawMaterialData();
	}

	return rawMaterialIt->second_;
}

Urho3D::SharedPtr<IGameChemistry> CreateGameChemistry()
{
	Urho3D::SharedPtr<IGameChemistry> gameChemistry;

	gameChemistry = new EngineChemistry(g_pApp->GetContext());
	if (gameChemistry.Get() && !gameChemistry->VInitialize())
	{
		// Physics failed to initialize. Delete it.
		gameChemistry.Reset();
	}

	return gameChemistry;
}

Urho3D::SharedPtr<IGameChemistry> CreateNullChemistry()
{
	Urho3D::SharedPtr<IGameChemistry> gameChemistry = Urho3D::SharedPtr<IGameChemistry>(new NullChemistry(g_pApp->GetContext()));
	if (gameChemistry.Get() && !gameChemistry->VInitialize())
	{
		// Physics failed to initialize. Delete it.
		gameChemistry.Reset();
	}

	return gameChemistry;

}