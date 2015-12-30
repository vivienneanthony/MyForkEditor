#pragma once


#include "../../MainComponent.h"

enum OSDeckType
{
	BaseDeck,
	SpaceShipDeck,
	DeckUNKNOWN,
};

class OSDeckInterface : public MainComponent
{
public:
	OSDeckInterface(Context* context);
	OSDeckInterface();
	virtual ~OSDeckInterface();
	
	// These should be implemented by specific OS Deck
	virtual const OSDeckType GetDeckType() = 0;
	virtual void SetGetDeckType(const OSDeckType& type) = 0;
};