#pragma once

#include "../TestFactory/Components/MainComponent.h"

class GameNode : public Node
{
	URHO3D_OBJECT(GameNode, Node)
public:
	typedef HashMap<ComponentId, StrongGameNodeComponentPtr> GameNodeComponents;

	GameNode(Context* context, GameNodeId gameNodeId);
	virtual ~GameNode();

	bool Init(pugi::xml_node* pData);
	void Destroy(void);
	void PostInit();


	void AddComponent(StrongGameNodeComponentPtr pComponent);

	//Getters/Setters
	String GetGameNodeType() const { return m_GameNodeType; }

public:
	GameNodeComponents m_Components;
	
	GameNodeType m_GameNodeType;
	String m_Resource;					// the XML file from which this actor was initialized (considered the "Archetype" file)
										// editor helpers, but will also be great for save game files if we ever make them.

	// template function for retrieving components
	template <class ComponentType>
	WeakPtr<ComponentType> GetComponent(ComponentId id)
	{
		GameNodeComponents::Iterator findIt = m_Components.Find(id);
		if (findIt != m_Components.End())
		{
			
			StrongGameNodeComponentPtr pBase(findIt->second_);
			WeakPtr<ComponentType> pWeakBase(pBase);
			return pWeakBase;
		}
		else
		{
			return WeakPtr<ComponentType>();
		}
	}

	template <class ComponentType>
	WeakPtr<ComponentType> GetComponent(String name)
	{
		ComponentId id = BaseComponent::GetIdFromName(name);
		GameNodeComponents::Iterator findIt = m_Components.Find(id);
		if (findIt != m_Components.End())
		{
			StrongGameNodeComponentPtr pBase(findIt->second_);
			WeakPtr<ComponentType> pWeakBase(pBase);
			return pWeakBase;
		}
		else
		{
			return WeakPtr<ComponentType>();
		}
	}

	const GameNodeComponents* GetComponents() { return &m_Components; }

};