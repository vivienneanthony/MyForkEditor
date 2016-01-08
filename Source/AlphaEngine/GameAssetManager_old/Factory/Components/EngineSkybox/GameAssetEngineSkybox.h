#ifndef GameAssetEngineSkybox_H
#define GameAssetEngineSkybox_H

// Base game asset component
#include "../../BaseComponent.h"

// Game Asset
class GameAssetEngineSkybox : public BaseComponent
{
    URHO3D_OBJECT(GameAssetEngineSkybox, BaseComponent);
public:
    static const GameAssetType g_Type;

	GameAssetEngineSkybox(Context* context);
	GameAssetEngineSkybox();
	virtual ~GameAssetEngineSkybox();

	// Base component overrided functions
	virtual bool VInit(const GameAsset* pGameAsset);
	virtual void VUpdateDelegate(float timeStep) { }
	virtual void VInitializeDelegate() { }

	virtual GameAssetType VGetGameAssetType(void) const { return g_Type; }

	// Additional
	virtual void Initialize(void);

    // Set scale
    virtual void SetScale(float SetScale) { this->GetNode()->SetScale(SetScale);}

private:
    String SkyboxModel;               // Skybox Model

    Skybox * m_pNodeSkybox;           // Save associated model
};

#endif // INTERACTIVE_H
