#pragma once



class TestFactory : public Object
{
	URHO3D_OBJECT(TestFactory, Object);

	typedef Vector<StrongGameNodeComponentPtr> GameNodeComponents;

public:
	TestFactory(Context* context);
	~TestFactory();


	// Node can be created on client side and server side.
	// On client side we get event from server, where is contained serversId.
	StrongNodePtr CreateNode(String resource, pugi::xml_node* overrides, const Matrix4* initTransform, const GameNodeId serversId);
	bool InitNode(StrongNodePtr node, pugi::xml_node* root);


	// Modify node
	void ModifyGameNode(StrongNodePtr node, pugi::xml_node* overrides, bool calledFromCreateNode = true);
		
	virtual StrongGameNodeComponentPtr VCreateComponent(pugi::xml_node* pData);

protected:
	GameNodeId GetNextGameNodeId() { ++m_LastGameNodeId; return m_LastGameNodeId; }
		

protected:
	GenericObjectFactory<MainComponent, ComponentId> m_ComponentFactory;

	GameNodeComponents m_Components;			// Components for current created GameNode. In order to call VPostInit()
	
	GameNodeId m_LastGameNodeId;
};