#pragma once

#include "../OSBaseModuleComponent/OSBaseModuleComponent.h"

class OSTrussTriple1Component : public OSBaseModuleComponent
{
	URHO3D_OBJECT(OSTrussTriple1Component , OSBaseModuleComponent)
public:
	const static String g_Name;
	OSTrussTriple1Component (Context* context);
	OSTrussTriple1Component ();
	virtual ~OSTrussTriple1Component ();

	// mainComponent interface
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

	// OSTrussTriple1Component  interface
	// There can be more methods.


protected:
	virtual bool VDelegateInit(pugi::xml_node* pData);

};