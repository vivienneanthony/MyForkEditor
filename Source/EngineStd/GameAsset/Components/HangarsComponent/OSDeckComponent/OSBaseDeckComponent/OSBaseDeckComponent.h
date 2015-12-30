#pragma once

#include "../OSDeckInterface.h"

class OSBaseDeckComponent : public OSDeckInterface
{
	URHO3D_OBJECT(OSBaseDeckComponent, OSDeckInterface)
public:
	const static String g_Name;
	OSBaseDeckComponent (Context* context);
	OSBaseDeckComponent ();
	virtual ~OSBaseDeckComponent ();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);
	// For editor (called by station). 
	static pugi::xml_node VGenerateXML(StrongNodePtr deckNode, pugi::xml_node root);


	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node pData) { return true; }

public:
	// OSDeckInterface
	virtual const OSDeckType GetDeckType() { return m_DeckType; }
	virtual void SetGetDeckType(const OSDeckType& type) { m_DeckType = type; }

protected:
	OSDeckType m_DeckType;					// Deck type
	String m_Name;								// Deck specific name
};