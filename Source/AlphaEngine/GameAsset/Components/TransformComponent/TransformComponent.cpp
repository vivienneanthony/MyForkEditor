#include "AlphaEngineStd.h"
#include "TransformComponent.h"

const String TransformComponent::g_Name = "TransformComponent";

TransformComponent::TransformComponent(Context* context) : MainComponent(context)
,m_Scale(Vector3::ONE)
,m_Position(Vector3::ZERO)
,m_Rotation(Quaternion::IDENTITY)
{

}

TransformComponent::TransformComponent() : MainComponent()
,m_Scale(Vector3::ONE)
,m_Position(Vector3::ZERO)
,m_Rotation(Quaternion::IDENTITY)
{

}

TransformComponent::~TransformComponent()
{

}

// mainComponent interface
bool TransformComponent::VInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("Position");
	if (node)
	{
		float x = node.attribute("x").as_float();
		float y = node.attribute("y").as_float();
		float z = node.attribute("z").as_float();
		m_Position = Vector3(x, y, z);
	}

	node = pData.child("Rotation");
	if (node)
	{

		float x_r = node.attribute("x").as_float();
		float y_r = node.attribute("y").as_float();
		float z_r = node.attribute("z").as_float();
		float w_r = node.attribute("w").as_float();
		m_Rotation = Quaternion(w_r, x_r, y_r, z_r);
	}

	node = pData.child("Scale");
	if (node)
	{
		float x_s = node.attribute("x").as_float();
		float y_s = node.attribute("y").as_float();
		float z_s = node.attribute("z").as_float();
		m_Scale = Vector3(x_s, y_s, z_s);
	}

	return true;
}

void TransformComponent::VPostInit(void)
{
	SetPosition(m_Position);
	SetRotation(m_Rotation);
	SetScale(m_Scale);
}

void TransformComponent::VUpdate(float timeStep)
{

}

void TransformComponent::VOnChanged(void)
{
	SetPosition(m_Position);
	SetRotation(m_Rotation);
	SetScale(m_Scale);
}

void TransformComponent::SetRotation(const Quaternion& quat)
{
	this->GetNode()->SetRotation(quat);
}

void TransformComponent::SetPosition(const Vector3& position)
{
	this->GetNode()->SetPosition(position);
}

void TransformComponent::SetScale(const Vector3& scale)
{
	this->GetNode()->SetScale(scale);
}


pugi::xml_node TransformComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node positionNode = componentNode.append_child("Position");
	Vector3 pos = GetPosition();
	pugi::xml_attribute positionAttribute = positionNode.append_attribute("x");
	positionAttribute.set_value(pos.x_);
	positionAttribute = positionNode.append_attribute("y");
	positionAttribute.set_value(pos.y_);
	positionAttribute = positionNode.append_attribute("z");
	positionAttribute.set_value(pos.z_);

	pugi::xml_node rotationNode = componentNode.append_child("Rotation");
	Quaternion quat = GetRotation();
	pugi::xml_attribute rotationAttribute = rotationNode.append_attribute("x");
	rotationAttribute.set_value(quat.x_);
	rotationAttribute = rotationNode.append_attribute("y");
	rotationAttribute.set_value(quat.y_);
	rotationAttribute = rotationNode.append_attribute("z");
	rotationAttribute.set_value(quat.z_);
	rotationAttribute = rotationNode.append_attribute("w");
	rotationAttribute.set_value(quat.w_);

	pugi::xml_node scaleNode = componentNode.append_child("Scale");
	Vector3 scale = GetScale();
	pugi::xml_attribute scaleAttribute = scaleNode.append_attribute("x");
	scaleAttribute.set_value(scale.x_);
	scaleAttribute = scaleNode.append_attribute("y");
	scaleAttribute.set_value(scale.y_);
	scaleAttribute = scaleNode.append_attribute("z");
	scaleAttribute.set_value(scale.z_);

	return componentNode;
}
