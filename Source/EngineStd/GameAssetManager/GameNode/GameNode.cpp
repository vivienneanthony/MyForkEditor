#include "EngineStd.h"
#include "GameNode.h"

GameNode::GameNode(Context* context, GameNodeId gameNodeId) : Node(context)
{
	SetID(gameNodeId);
}

GameNode::~GameNode()
{
	URHO3D_LOGDEBUG("Destroying Game Node " + String(GetID()));
	assert(m_Components.Empty());		// if this assert fires, the game node was destroyed without calling GameNode::Destroy()
}

bool GameNode::Init(pugi::xml_node* pData)
{
	URHO3D_LOGDEBUG("Initializing GameNode with id = " + String(GetID()));

	if (pData)
	{
		m_GameNodeType = pData->attribute("type").as_string();
		m_Resource = pData->attribute("resource").as_string();
		return true;
	}
	
	return false;
}

void GameNode::PostInit()
{
	for (GameNodeComponents::Iterator it = m_Components.Begin(); it != m_Components.End(); ++it)
	{
		it->second_->VPostInit();
	}
}

void GameNode::Destroy(void)
{
	m_Components.Clear();
}

void GameNode::AddComponent(StrongGameNodeComponentPtr pComponent)
{
	GameNodeComponents::Iterator success = m_Components.Insert(MakePair(pComponent->VGetId(), pComponent));
	assert(success->second_);
}
