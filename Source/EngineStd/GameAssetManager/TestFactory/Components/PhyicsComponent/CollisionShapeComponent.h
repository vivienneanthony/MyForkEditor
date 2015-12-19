#pragma once

#include "../MainComponent.h"

class CollisionShapeComponent : public MainComponent
{
	URHO3D_OBJECT(CollisionShapeComponent, MainComponent)

public:
	const static String g_Name;

	CollisionShapeComponent();
	virtual ~CollisionShapeComponent();


};