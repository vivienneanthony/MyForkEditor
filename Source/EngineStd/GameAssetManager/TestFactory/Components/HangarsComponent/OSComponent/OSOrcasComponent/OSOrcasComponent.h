#pragma once

#include "../OSBaseComponent/OSBaseComponent.h"

class OSOrcasComponent : public OSBaseComponent
{
	URHO3D_OBJECT(OSOrcasComponent, OSBaseComponent)

	typedef Vector<StrongNodePtr> StationModules;

public:
	const static String g_Name;
	OSOrcasComponent (Context* context);
	OSOrcasComponent ();
	virtual ~OSOrcasComponent ();

	// mainComponent interface
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

protected:
	virtual bool VDelegateInit(pugi::xml_node* pData);

protected:



};