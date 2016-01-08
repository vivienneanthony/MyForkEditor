#ifndef GameAssetComponent_H
#define GameAssetComponent_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetComponent : public BaseComponent
{
    URHO3D_OBJECT(GameAssetComponent, BaseComponent);

public:
	GameAssetComponent(Context* context);
    GameAssetComponent();
    virtual ~GameAssetComponent();

	static const GameAssetType g_Type;

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

};

#endif // INTERACTIVE_H
