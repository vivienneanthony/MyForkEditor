#pragma once

#include "RawMaterial/RawMaterialData.h"

class EngineChemistry : public IGameChemistry
{
	URHO3D_OBJECT(EngineChemistry, IGameChemistry)

	// Tables read from the XML by name
	typedef HashMap<String, RawMaterialData> RawMaterialTable;

public:
	EngineChemistry(Context* context);
	virtual ~EngineChemistry();

	// Load data from XML files
	void LoadXML();

	// Initialiazation and Maintenance of
	virtual bool VInitialize();
	virtual void VOnUpdate(float timeStep);

	// Look in tables
	RawMaterialData LookupRawMaterialInfo(const String& rawMaterialStr);

protected:
	RawMaterialTable m_RawMaterialTable;

};

extern Urho3D::SharedPtr<IGameChemistry> CreateGameChemistry();
extern Urho3D::SharedPtr<IGameChemistry> CreateNullChemistry();