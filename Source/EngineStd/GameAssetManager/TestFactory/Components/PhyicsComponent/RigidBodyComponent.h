#pragma once

#include "../MainComponent.h"

class RigidBodyComponent : public MainComponent
{
	URHO3D_OBJECT(RigidBodyComponent, MainComponent)

public:
	const static String g_Name;

	RigidBodyComponent();
	virtual ~RigidBodyComponent();


};