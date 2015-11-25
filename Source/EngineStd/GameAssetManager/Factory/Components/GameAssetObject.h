#ifndef GameAssetObject_H
#define GameAssetObject_H

// Base game object
#include "../BaseComponent.h"

// Game Asset
class GameAssetObject : public BaseComponent
{
    URHO3D_OBJECT(GameAssetObject, BaseComponent);
public:
    static const GameAssetType g_Type;

	GameAssetObject(Context* context);
	GameAssetObject();
	virtual ~GameAssetObject();

	// Base component overrided functions
	virtual bool VInit(GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

};

#endif // INTERACTIVE_H
