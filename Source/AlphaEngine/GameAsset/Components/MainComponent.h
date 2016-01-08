#pragma once

class MainComponent : public LogicComponent
{
	URHO3D_OBJECT(MainComponent, LogicComponent)
	friend class GAFactory;

public:
	const static String g_Name;

	MainComponent(Context* context);
	MainComponent();
	virtual ~MainComponent();

	// These function called by LogicComponent
	virtual void FixedUpdate(float timeStep);

	// These functions are meant to be overridden by the implementation classes of the components.
	virtual bool VInit(pugi::xml_node pData) { return true; }
	virtual void VPostInit(void) { }
	virtual void VUpdate(float timeStep) { }
	virtual void VOnChanged(void) { }	

	// These function can be used for edtior
	// In order to save game node in .XML file.
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root) { return root; }

	virtual ComponentId VGetId(void) const { return GetIdFromName(VGetName()); }
	virtual String VGetName() const { return g_Name; }

	CreateMode GetCreateMode() const { return m_CreateMode; }

	static ComponentId GetIdFromName(String g_Name)
	{
		return StringHash(g_Name).ToHash();
	}

	static ComponentId GetUniqueIdForEngine(int nodeId, String componentName)
	{
		return StringHash(String(nodeId) + componentName).ToHash();
	}

	static ComponentId GetUniqueIdForURHO(int nodeId, String componentName, int number)
	{
		return StringHash(String(nodeId) + componentName + String(number)).ToHash();
	}


protected:
	Urho3D::CreateMode m_CreateMode;		// Local or replicated ?

};