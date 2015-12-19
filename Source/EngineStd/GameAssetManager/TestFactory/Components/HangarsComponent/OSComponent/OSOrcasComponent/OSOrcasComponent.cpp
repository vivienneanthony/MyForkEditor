#include "EngineStd.h"
#include "OSOrcasComponent.h"

#include "GameLogic/BaseGameLogic.h"
#include "EventManager/Local/LocalEvents.h"

const String OSOrcasComponent::g_Name = String("OSOrcasComponent");

OSOrcasComponent::OSOrcasComponent(Context* context) : OSBaseComponent(context)
{
	m_StationType = OrcasStation;
}

OSOrcasComponent::OSOrcasComponent() : OSBaseComponent()
{
	m_StationType = OrcasStation;
}

OSOrcasComponent::~OSOrcasComponent ()
{

}

bool OSOrcasComponent::VDelegateInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("OSOrcas");
	if (node)
	{
		// get orcas station specific information
	}
	return true;
}

void OSOrcasComponent::VUpdate(float timeStep)
{
	OSBaseComponent::VUpdate(timeStep);

}

void OSOrcasComponent::VOnChanged(void)
{
	OSBaseComponent::VOnChanged();


}

