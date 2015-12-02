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

	// Additional
	virtual void Initialize(void);

	// Modify node
    virtual void SetViewMask (unsigned SetMask) { m_pNodeStaticModel->SetViewMask(SetMask);}
    virtual void SetLightMask (unsigned SetMask) { m_pNodeStaticModel->SetLightMask(SetMask);}
    virtual void SetShadowMask (unsigned SetMask) { m_pNodeStaticModel->SetShadowMask(SetMask);}
    virtual void SetZoneMask (unsigned SetMask) { m_pNodeStaticModel->SetZoneMask(SetMask);}
    virtual void SetCastShadow (unsigned SetShadow) {m_pNodeStaticModel->SetCastShadows(SetShadow);}
    virtual void SetScale      (float SetScale) {this->GetNode()->SetScale(SetScale);}

private:
    bool bPhysicalObject;                       // If Object is a model
    String PhysicalModel;                       // Model filename

    StaticModel * m_pNodeStaticModel;           // Save associated model
};

#endif // INTERACTIVE_H
