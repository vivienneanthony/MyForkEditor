#include <HangarsServerStd.h>
#include "HangarsGameLogic.h"


HangarsGameLogic::HangarsGameLogic(Context* context) : BaseGameLogic(context)
{


}

HangarsGameLogic::~HangarsGameLogic()
{


}

bool HangarsGameLogic::VInitialize()
{
	BaseGameLogic::VInitialize();

	return true;
}


void HangarsGameLogic::VShutdown()
{
	BaseGameLogic::VShutdown();
}


void HangarsGameLogic::VChangeState(enum BaseGameState newState)
{
	BaseGameLogic::VChangeState(newState);
}

bool HangarsGameLogic::VLoadGameDelegate(String pLevelData)
{
	return true;
}