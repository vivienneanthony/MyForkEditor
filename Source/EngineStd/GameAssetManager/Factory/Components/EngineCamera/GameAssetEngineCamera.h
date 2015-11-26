#ifndef GameAssetEngineCamera_H
#define GameAssetEngineCamera_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetEngineCamera : public BaseComponent
{
    URHO3D_OBJECT(GameAssetEngineCamera, BaseComponent);
public:
    // Construct.
	GameAssetEngineCamera(Context* context);
    GameAssetEngineCamera();
    virtual ~GameAssetEngineCamera();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

    // Load components
    virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
