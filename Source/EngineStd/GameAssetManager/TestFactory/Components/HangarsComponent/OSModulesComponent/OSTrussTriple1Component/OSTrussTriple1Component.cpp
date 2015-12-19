#include "EngineStd.h"
#include "OSTrussTriple1Component.h"

const String OSTrussTriple1Component ::g_Name = String("OSTrussTriple1Component");

OSTrussTriple1Component ::OSTrussTriple1Component (Context* context) : OSBaseModuleComponent(context)
{
	m_ModuleType = TrussTriple1;
}

OSTrussTriple1Component ::OSTrussTriple1Component () : OSBaseModuleComponent()
{
	m_ModuleType= TrussTriple1;
}

OSTrussTriple1Component ::~OSTrussTriple1Component ()
{

}

bool OSTrussTriple1Component ::VDelegateInit(pugi::xml_node* pData)
{

	return true;
}

void OSTrussTriple1Component ::VPostInit(void)
{
	OSBaseModuleComponent::VPostInit();

}

void OSTrussTriple1Component ::VUpdate(float timeStep)
{
	OSBaseModuleComponent::VUpdate(timeStep);

}

void OSTrussTriple1Component ::VOnChanged(void)
{
	OSBaseModuleComponent::VOnChanged();

}