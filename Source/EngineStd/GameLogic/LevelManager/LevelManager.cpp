#include "EngineStd.h"

#include "../BaseGameLogic.h"

#include "EventManager/Local/LocalEvents.h"

//#include "GameAssetManager/GameNode/GameNode.h"
#include "../../Resources/XMLResourceLoader/XMLResourceLoader.h"

#include "LevelManager.h"

LevelManager::LevelManager(Context* context, BaseGameLogic* gameLogic) : Object(context)
{
	m_pGameLogic = gameLogic;
}

LevelManager::~LevelManager()
{


}

bool LevelManager::Initialize(Vector<String> &levels)
{
	Vector<String>::Iterator i = levels.Begin();
	while (i != levels.End())
	{
		m_Levels.Push(*i);
		++i;
	}
	return true;
}

void LevelManager::Shutdown()
{


}

bool LevelManager::CreateLevel(SharedPtr<Scene> scene, String levelName, bool addToMainScene)
{
	ResourceCache* cache = g_pApp->GetConstantResCache();

	pugi::xml_document* document = XMLResourceLoader::LoadXMLDocument(levelName);

	if (!document)
	{
		return false;
	}

	pugi::xml_node root = document->first_child();

	if (!root)
	{
		return false;
	}


	// load all initial game nodes
	pugi::xml_node node = root.child("StaticGameAssets");
	if (node)
	{
		for (pugi::xml_node pNode = node.first_child(); pNode; pNode = pNode.next_sibling())
		{
			String resource = pNode.attribute("resource").as_string();

			StrongNodePtr pGameNode = m_pGameLogic->VCreateGameNode(resource, pNode, NULL, INVALID_GAME_NODE_ID, addToMainScene);
			if (pGameNode)
			{
				Event_Data_New_Game_Node pNewGameNodeEvent(pGameNode->GetID());
				SendEvent(Event_Data_New_Game_Node::g_EventType, pNewGameNodeEvent.VSerialize());
			}

			GameNodeId nodeId = pGameNode->GetID();
			String name = pGameNode->GetName();

			if (!addToMainScene)
			{
				scene->AddChild(pGameNode);
				
			}
		}
	}


	return true;
}

