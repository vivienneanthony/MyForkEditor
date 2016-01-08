#pragma once

#include "../OSModuleInterface.h"

class OSBaseModuleComponent : public OSModuleInterface
{
	URHO3D_OBJECT(OSBaseModuleComponent, OSModuleInterface)
public:
	const static String g_Name;
	OSBaseModuleComponent (Context* context);
	OSBaseModuleComponent ();
	virtual ~OSBaseModuleComponent ();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);
	
	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);
	// For editor (called by station). 
	static pugi::xml_node VGenerateXML(StrongNodePtr moduleNode, pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }


protected:
	virtual bool VDelegateInit(pugi::xml_node pData) { return true; }

public:
	// OSModuleInterface
	virtual void SetLevel(int value) { m_Level = value; }
	virtual int GetLevel() { return m_Level; }

	virtual void Upgrade(VariantMap& materials) { }
	virtual void Destroy() { }

	virtual OSModuleType GetModuleType() { return m_ModuleType; }
	virtual void SetModuleType(OSModuleType type) { m_ModuleType = type; }

protected:
	OSModuleType m_ModuleType;				//  Module type
	String m_Name;										//  Module specific name

	int m_Level;										//	Module level

};