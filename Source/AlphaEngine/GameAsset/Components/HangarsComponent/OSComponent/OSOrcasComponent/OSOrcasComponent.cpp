#include "AlphaEngineStd.h"
#include "OSOrcasComponent.h"

#include "../../OSModuleComponent/OSBaseModuleComponent/OSBaseModuleComponent.h"

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

bool OSOrcasComponent::VDelegateInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("OSOrcas");
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

pugi::xml_node OSOrcasComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = OSBaseComponent::VGenerateXML(root);

	pugi::xml_node orcasNode = componentNode.append_child("OSOrcas");

	return componentNode;
}

