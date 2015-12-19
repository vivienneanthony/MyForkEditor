#include "EngineStd.h"
#include "TransformComponent.h"

const String TransformComponent::g_Name = "TransformComponent";

TransformComponent::TransformComponent(Context* context) : MainComponent(context)
{

}

TransformComponent::TransformComponent() : MainComponent()
{

}

TransformComponent::~TransformComponent()
{

}

// mainComponent interface
bool TransformComponent::VInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("Position");
	if (node)
	{
		float x = node.attribute("x").as_float();
		float y = node.attribute("y").as_float();
		float z = node.attribute("z").as_float();
		m_Position = Vector3(x, y, z);
	}

	node = pData->child("Rotation");
	if (node)
	{

		float x_r = node.attribute("x").as_float();
		float y_r = node.attribute("y").as_float();
		float z_r = node.attribute("z").as_float();
		float w_r = node.attribute("w").as_float();
		m_Rotation = Quaternion(w_r, x_r, y_r, z_r);
	}

	node = pData->child("Scale");
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


void TransformComponent::VOnChanged(void)
{
	SetPosition(m_Position);
	SetRotation(m_Rotation);
	SetScale(m_Scale);
}