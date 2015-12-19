#pragma once

#include "../../MainComponent.h"

class BaseLightComponent : public MainComponent
{
	URHO3D_OBJECT(BaseLightComponent, MainComponent)

public:
	const static String g_Name;

	BaseLightComponent(Context* context);
	BaseLightComponent();
	virtual ~BaseLightComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node* pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	virtual String VGetName() const { return g_Name; }

protected:
	// Passed Urho3D parameters
	void SetLightType(LightType SetLight)			{ m_pNodeLight->SetLightType(SetLight); }
	void SetBrightness(float SetBright)				{ m_pNodeLight->SetBrightness(SetBright); }
	void SetColor(Color& SetColor)                  { m_pNodeLight->SetColor(SetColor); }
	void SetSpecularIntensity(float SetSpecular)    { m_pNodeLight->SetSpecularIntensity(SetSpecular); }
	void SetRange(float SetRange)                   { m_pNodeLight->SetRange(SetRange); }
	void SetFOV(float SetFOV)                       { m_pNodeLight->SetFov(SetFOV); }
	void SetLookAt(Vector3 SetLookAt)               { GetNode()->LookAt(SetLookAt); }
	void SetCastShadows(bool SetCastShadows)        { m_pNodeLight->SetCastShadows(SetCastShadows); }

	// Set Masking
	void SetViewMask(unsigned SetMask)				{ m_pNodeLight->SetViewMask(SetMask); }
	void SetLightMask(unsigned SetMask)				{ m_pNodeLight->SetLightMask(SetMask); }
	void SetShadowMask(unsigned SetMask)			{ m_pNodeLight->SetShadowMask(SetMask); }
	void SetZoneMask(unsigned SetMask)				{ m_pNodeLight->SetZoneMask(SetMask); }

protected:
	Light* m_pNodeLight;							// Save associated Light component

	float m_Brightness;
	float m_Specular;
	Vector3 m_LookAt;
	LightType m_LightType;
	unsigned int m_LightMask;
	bool m_bCastShadow;

	static unsigned int m_LightCount;				// How many lights has this node ?
};