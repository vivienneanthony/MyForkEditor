#pragma once


#include "../../MainComponent.h"

typedef Vector<StrongNodePtr> StationRooms;

enum OSRoomType
{
	BaseRoom,
	ReserveRoom,
	RoomUNKNOWN,
};

class OSRoomInterface : public MainComponent
{
public:
	OSRoomInterface(Context* context);
	OSRoomInterface();
	virtual ~OSRoomInterface();
	
	// These should be implemented by specific OS Room
	virtual const OSRoomType GetRoomType() = 0;
	virtual void SetRoomType(const OSRoomType& type) = 0;
};