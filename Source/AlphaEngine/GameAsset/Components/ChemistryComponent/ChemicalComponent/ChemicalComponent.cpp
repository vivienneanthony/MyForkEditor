#include "AlphaEngineStd.h"
#include "ChemicalComponent.h"

const String ChemicalComponent::g_Name = "ChemicalComponent";

ChemicalComponent::ChemicalComponent(Context* context) : MainComponent(context)
{

}

ChemicalComponent::ChemicalComponent() : MainComponent()
{

}

ChemicalComponent::~ChemicalComponent()
{

}

// mainComponent interface
bool ChemicalComponent::VInit(pugi::xml_node pData)
{
	
	
	return true;
}

void ChemicalComponent::VPostInit(void)
{
	MainComponent::VPostInit();
}

void ChemicalComponent::VUpdate(float timeStep)
{
	MainComponent::VUpdate(timeStep);
}

void ChemicalComponent::VOnChanged(void)
{
	MainComponent::VOnChanged();
}