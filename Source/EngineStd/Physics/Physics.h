#pragma once

extern Urho3D::SharedPtr<IGamePhysics> CreateGamePhysics();
extern Urho3D::SharedPtr<IGamePhysics> CreateNullPhysics();

#include "Material/MaterialData.h"

class EnginePhysics : public IGamePhysics
{
	URHO3D_OBJECT(EnginePhysics, IGamePhysics)

	// Tables read from the XML
	typedef HashMap<String, float> DensityTable;
	typedef HashMap<String, MaterialData> MaterialTable;
		
public:
	EnginePhysics(Context* context);
	virtual ~EnginePhysics();

	// Load data from XML files
	void LoadXML();

	// Initialiazation and Maintenance of the Physics World
	virtual bool VInitialize();
	virtual void VSyncVisibleScene();
	virtual void VOnUpdate(float timeStep);

	// Debugging
	virtual void VRenderDiagnostics();

	// Look in tables
	float LookupSpecificGravity(const String& densityStr);
	MaterialData LookupMaterialData(const String& materialStr);

protected:
	MaterialTable m_MaterialTable;
	DensityTable m_DensityTable;
};


