#ifndef GameAssetEngineLight_H
#define GameAssetEngineLight_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetEngineLight : public BaseComponent
{
    URHO3D_OBJECT(GameAssetEngineLight, BaseComponent);
public:
    // Construct.
	GameAssetEngineLight(Context* context);
    GameAssetEngineLight();
    virtual ~GameAssetEngineLight();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

	// Base component
	virtual void Initialize(void);

private:

};

#endif // INTERACTIVE_H
