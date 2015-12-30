#pragma once

#include "../MainComponent.h"

class TransformComponent : public MainComponent
{
	URHO3D_OBJECT(TransformComponent, MainComponent)

public:
	const static String g_Name;
	
	TransformComponent(Context* context);
	TransformComponent();
	virtual ~TransformComponent();

	// mainComponent interface
	virtual bool VInit(pugi::xml_node pData);
	virtual void VPostInit(void);
	virtual void VUpdate(float timeStep);
	virtual void VOnChanged(void);

	// For editor
	virtual pugi::xml_node VGenerateXML(pugi::xml_node root);

	virtual String VGetName() const { return g_Name; }

	const Vector3& GetScale() const { return m_Scale; }
	const Vector3& GetPosition() const { return m_Position; }
	const Quaternion& GetRotation() const { return m_Rotation; }

protected:
	void SetRotation(const Quaternion& quat);
	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);

protected:
	Vector3 m_Position;
	Quaternion m_Rotation;
	Vector3 m_Scale;
};