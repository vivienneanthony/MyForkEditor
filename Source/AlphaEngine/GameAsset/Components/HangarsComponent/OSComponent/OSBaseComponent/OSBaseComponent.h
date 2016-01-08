#pragma once

#include "../OSInterface.h"

class OSBaseComponent : public OSInterface
{
	URHO3D_OBJECT(OSBaseComponent, OSInterface)

public:
	const static String g_Name;
	OSBaseComponent (Context* context);
	OSBaseComponent ();
	virtual ~OSBaseComponent ();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node pData) { return true; }

public:
	// OSInterface interface
	// There can be more methods.
	// Station
	virtual void Upgrade(VariantMap& materials);
	virtual void Destroy();

	const OSType GetStationType() { return m_StationType; }
	void SetStationType(const OSType& value) { m_StationType = value; }

	// Modules
	virtual const StationModules& GetAllModules() { return m_StationModules; }
	virtual const StationModules& GetModules(const OSModuleType& type);
	virtual unsigned int GetStationModulesCount() { return m_ModulesCount; }

	virtual bool CreateModule(pugi::xml_node pData);

	virtual void DestroyModule(const GameNodeId& id);
	virtual void DestroyModule(const OSModuleType& type);
	virtual void DestroyModule(StrongNodePtr pModule);

	// Rooms
	virtual const StationRooms& GetAllRooms() { return m_StationRooms; }
	virtual const StationRooms& GetRooms(const OSRoomType& type);
	virtual unsigned int GetStationRoomsCount() { return m_RoomsCount; }

	virtual bool CreateRoom(pugi::xml_node pData);

	virtual void DestroyRoom(const GameNodeId& id);
	virtual void DestroyRoom(const OSRoomType& type);
	virtual void DestroyRoom(StrongNodePtr pModule);

	// Deck
	virtual const StationDecks& GetAllDecks() { return m_StationDecks; }
	virtual const StationDecks& GetDecks(const OSDeckType& type);
	virtual unsigned int GetStationDecksCount() { return m_DecksCount; }

	virtual bool CreateDeck(pugi::xml_node pData);

	virtual void DestroyDeck(const GameNodeId& id);
	virtual void DestroyDeck(const OSDeckType& type);
	virtual void DestroyDeck(StrongNodePtr pModule);
	
protected:
	// Delegates
	virtual void VInitializeAllDelegates() { }
	virtual void VDestroyAllDelegates() { }

private:
	StrongNodePtr CreateStationComponent(pugi::xml_node node);

protected:
	StationModules m_StationModules;				// All modules that have this station
	StationRooms m_StationRooms;					// All rooms that have this station
	StationDecks m_StationDecks;					// All decks that have this station

	OSType m_StationType;							// OriginStatio type
	String m_Name;									// Station name

	static unsigned int m_ModulesCount;				// How many modules have this station ?
	static unsigned int m_RoomsCount;				// How many rooms have this station ?
	static unsigned int m_DecksCount;				// How many decks have this station ?

private:
	StationModules m_TempStationModules;			// If method get modules if called. We have to put all modules in this vector.
	StationRooms m_TempStationRooms;				// If method get modules if called. We have to put all rooms in this vector.
	StationDecks m_TempStationDecks;				// If method get modules if called. We have to put all decks in this vector.
};