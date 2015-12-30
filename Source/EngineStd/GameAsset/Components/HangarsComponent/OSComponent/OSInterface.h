#pragma once


#include "../../MainComponent.h"

#include "../OSModuleComponent/OSModuleInterface.h"
#include "../OSDeckComponent/OSDeckInterface.h"
#include "../OSRoomComponent/OSRoomInterface.h"

typedef Vector<StrongNodePtr> StationModules;
typedef Vector<StrongNodePtr> StationRooms;
typedef Vector<StrongNodePtr> StationDecks;



enum OSType
{
	BaseStation,
	OrcasStation,
	TerranStation,
	VordonsStation,
	OSUNKNOWN,
};

class OSInterface : public MainComponent
{
public:
	OSInterface(Context* context);
	OSInterface();
	virtual ~OSInterface();
	
	// These should be implemented by specific OS
	virtual void Upgrade(VariantMap& materials) = 0;
	virtual void Destroy() = 0;

	virtual const OSType GetStationType() = 0;
	virtual void SetStationType(const OSType& type) = 0;

	// Modules
	virtual const StationModules& GetAllModules() = 0;
	virtual const StationModules& GetModules(const OSModuleType& type) = 0;
	virtual unsigned int GetStationModulesCount() = 0;
	
	virtual bool CreateModule(pugi::xml_node pData) = 0;

	virtual void DestroyModule(const GameNodeId& id) = 0;
	virtual void DestroyModule(const OSModuleType& type) = 0;
	virtual void DestroyModule(StrongNodePtr pModule) = 0;

	// Rooms
	virtual const StationRooms& GetAllRooms() = 0;
	virtual const StationRooms& GetRooms(const OSRoomType& type) = 0;
	virtual unsigned int GetStationRoomsCount() = 0;

	virtual bool CreateRoom(pugi::xml_node pData) = 0;

	virtual void DestroyRoom(const GameNodeId& id) = 0;
	virtual void DestroyRoom(const OSRoomType& type) = 0;
	virtual void DestroyRoom(StrongNodePtr pModule) = 0;

	// Deck
	virtual const StationDecks& GetAllDecks() = 0;
	virtual const StationDecks& GetDecks(const OSDeckType& type) = 0;
	virtual unsigned int GetStationDecksCount() = 0;

	virtual bool CreateDeck(pugi::xml_node pData) = 0;

	virtual void DestroyDeck(const GameNodeId& id) = 0;
	virtual void DestroyDeck(const OSDeckType& type) = 0;
	virtual void DestroyDeck(StrongNodePtr pModule) = 0;
};