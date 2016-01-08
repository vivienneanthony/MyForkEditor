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
    virtual bool VInit(const GameAsset* pGameAsset);
    virtual void VUpdateDelegate(float timeStep) { }
    virtual void VInitializeDelegate() { }

    virtual GameAssetType VGetGameAssetType(void) const
    {
        return g_Type;
    }

    // Base component initialization
    virtual void Initialize(void);

    // Passed Urho3D parameters
    virtual void SetLightType(LightType SetLight)           { m_pNodeLight->SetLightType(SetLight); }
    virtual void SetBrightness(float SetBright)            { m_pNodeLight->SetBrightness(SetBright); }
    virtual void SetColor(Color &SetColor)                  { m_pNodeLight->SetColor(SetColor); }
    virtual void SetSpecularIntensity(float SetSpecular)   { m_pNodeLight->SetSpecularIntensity(SetSpecular); }
    virtual void SetRange (float SetRange)                 { m_pNodeLight->SetRange(SetRange);}
    virtual void SetFOV (float SetFOV)                     { m_pNodeLight->SetFov(SetFOV);}
    virtual void SetLookAt (Vector3 SetLookAt)              { this->GetNode()->LookAt(SetLookAt);}
    virtual void SetCastShadows (bool SetCastShadows)       { m_pNodeLight->SetCastShadows(SetCastShadows);}

    // Set Masking
    virtual void SetViewMask (unsigned SetMask) { m_pNodeLight->SetViewMask(SetMask);}
    virtual void SetLightMask (unsigned SetMask) { m_pNodeLight->SetLightMask(SetMask);}
    virtual void SetShadowMask (unsigned SetMask) { m_pNodeLight->SetShadowMask(SetMask);}
    virtual void SetZoneMask (unsigned SetMask) { m_pNodeLight->SetZoneMask(SetMask);}

private:
    Light * m_pNodeLight;                                       // Save associated Light component
};

#endif // INTERACTIVE_H
