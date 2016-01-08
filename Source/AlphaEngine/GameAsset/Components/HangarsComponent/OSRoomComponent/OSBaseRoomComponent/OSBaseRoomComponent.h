#pragma once

#include "../OSRoomInterface.h"

class OSBaseRoomComponent : public OSRoomInterface
{
	URHO3D_OBJECT(OSBaseRoomComponent, OSRoomInterface)
public:
	const static String g_Name;
	OSBaseRoomComponent (Context* context);
	OSBaseRoomComponent ();
	virtual ~OSBaseRoomComponent ();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);
	// For editor (called by station). 
	static pugi::xml_node VGenerateXML(StrongNodePtr roomNode, pugi::xml_node root);


	virtual String VGetName() const { return g_Name; }
	
protected:
	virtual bool VDelegateInit(pugi::xml_node pData) { return true; }

public:
	// OSRoomInterface
	virtual const OSRoomType GetRoomType() { return m_RoomType; }
	virtual void SetRoomType(const OSRoomType& type) { m_RoomType = type; }

protected:
	OSRoomType m_RoomType;				//  Room type
	String m_Name;							//  Module specific name

};