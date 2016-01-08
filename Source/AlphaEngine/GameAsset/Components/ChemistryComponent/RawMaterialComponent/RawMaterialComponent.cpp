#include "AlphaEngineStd.h"
#include "GameLogic/BaseGameLogic.h"
#include "RawMaterialComponent.h"

const String RawMaterialComponent::g_Name = "RawMaterialComponent";

RawMaterialComponent::RawMaterialComponent(Context* context) : MainComponent(context)
{
	m_RawMaterialData = RawMaterialData(String::EMPTY, String::EMPTY, 0);
}

RawMaterialComponent::RawMaterialComponent() : MainComponent()
{
	m_RawMaterialData = RawMaterialData(String::EMPTY, String::EMPTY, 0);
}

RawMaterialComponent::~RawMaterialComponent()
{

}

bool RawMaterialComponent::VInit(pugi::xml_node pData)
{
	// No point in having a raw material component with no game chemistry
	m_pGameChemistry = g_pApp->GetGameLogic()->VGetGameChemistry();
	if (!m_pGameChemistry)
	{
		return false;
	}

	return true;
}

void RawMaterialComponent::VPostInit(void)
{
	MainComponent::VPostInit();
}

void RawMaterialComponent::VUpdate(float timeStep)
{
	MainComponent::VUpdate(timeStep);
}

void RawMaterialComponent::VOnChanged(void)
{
	MainComponent::VOnChanged();
}