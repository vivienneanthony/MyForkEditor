#ifndef GameAssetEngineObject_H
#define GameAssetEngineObject_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetEngineObject : public BaseComponent
{
    URHO3D_OBJECT(GameAssetEngineObject, BaseComponent);
public:
    static const GameAssetType g_Type;

	GameAssetEngineObject(Context* context);
	GameAssetEngineObject();
	virtual ~GameAssetEngineObject();

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

};

#endif // INTERACTIVE_H
