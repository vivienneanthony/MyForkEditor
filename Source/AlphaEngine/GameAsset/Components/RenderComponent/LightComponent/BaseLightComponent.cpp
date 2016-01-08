#include "AlphaEngineStd.h"
#include "BaseLightComponent.h"

const String BaseLightComponent::g_Name = String("BaseLightComponent");

unsigned int BaseLightComponent::m_LightCount = 0;

BaseLightComponent::BaseLightComponent(Context* context) : MainComponent(context)
{
	m_pNodeLight = NULL;
	m_Brightness = 0.0f;
	m_Specular = 0.0f;
	m_LookAt = Vector3::ZERO;
	m_LightType = LightType::LIGHT_DIRECTIONAL;
	m_LightMask = 1;
	m_bCastShadow = false;
}

BaseLightComponent::BaseLightComponent()
{
	m_pNodeLight = NULL;
	m_Brightness = 0.0f;
	m_Specular = 0.0f;
	m_LookAt = Vector3::ZERO;
	m_LightType = LightType::LIGHT_DIRECTIONAL;
	m_LightMask = 1;
	m_bCastShadow = false;
}

BaseLightComponent::~BaseLightComponent()
{	
	m_LightCount--;
}

// mainComponent interface
bool BaseLightComponent::VInit(pugi::xml_node pData)
{
	m_LightCount++;
	
	pugi::xml_node node = pData.child("Brightness");
	if (node)
	{
		m_Brightness = node.attribute("value").as_float();
	}

	node = pData.child("Specular");
	if (node)
	{
		m_Specular = node.attribute("value").as_float();
	}

	node = pData.child("Type");
	if (node)
	{
		String value = node.attribute("value").as_string();
		if (value == "DIRECTIONAL")
		{
			m_LightType = LightType::LIGHT_DIRECTIONAL;
		}
		else if (value == "SPOT")
		{
			m_LightType = LightType::LIGHT_SPOT;
		}
		else if (value == "POINT")
		{
			m_LightType = LightType::LIGHT_POINT;
		}
		else
		{
			URHO3D_LOGERROR("Invalid light type in XML file");
		}
	}

	node = pData.child("CastShadow");
	if (node)
	{
		String value = node.attribute("value").as_string();
		m_bCastShadow = (value != "yes") ? true : false;
	}

	node = pData.child("LightMask");
	if (node)
	{
		m_LightMask = node.attribute("value").as_uint();
	}

	node = pData.child("LookAt");
	if (node)
	{
		float x = node.attribute("x").as_float();
		float y = node.attribute("y").as_float();
		float z = node.attribute("z").as_float();
		m_LookAt = Vector3(x, y, z);
	}

	

	return true;
}

void BaseLightComponent::VPostInit(void)
{
	m_pNodeLight = this->GetNode()->CreateComponent<Light>(m_CreateMode, MainComponent::GetUniqueIdForURHO(GetNode()->GetID(), g_Name, m_LightCount));
	if (!m_pNodeLight)
	{
		URHO3D_LOGERROR("Failed to create Urho3D::Light component in BaseLightComponent to Node with ID " + String(GetNode()->GetID()));
	}
	else
	{
		SetBrightness(m_Brightness);
		SetSpecularIntensity(m_Specular);
		SetLightType(m_LightType);
		SetCastShadows(m_bCastShadow);
		SetLightMask(m_LightMask);
		SetLookAt(m_LookAt);
	}
}

void BaseLightComponent::VUpdate(float timeStep)
{

}

void BaseLightComponent::VOnChanged(void)
{
	if (m_pNodeLight)
	{
		SetBrightness(m_Brightness);
		SetSpecularIntensity(m_Specular);
		SetLightType(m_LightType);
		SetCastShadows(m_bCastShadow);
		SetLightMask(m_LightMask);
		SetLookAt(m_LookAt);
	}
}

pugi::xml_node BaseLightComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node childNode = componentNode.append_child("Brightness");
	pugi::xml_attribute attribute = childNode.append_attribute("value");
	attribute.set_value(m_Brightness);
	
	childNode = componentNode.append_child("Specular");
	attribute = childNode.append_attribute("value");
	attribute.set_value(m_Specular);

	childNode = componentNode.append_child("Type");
	attribute = childNode.append_attribute("value");
	if (m_LightType == LightType::LIGHT_DIRECTIONAL)
	{
		attribute.set_value("DIRECTIONAL");
	}
	else if (m_LightType == LightType::LIGHT_SPOT)
	{
		attribute.set_value("SPOT");
	}
	else if (m_LightType == LightType::LIGHT_POINT)
	{
		attribute.set_value("POINT");
	}
	else
	{
		URHO3D_LOGERROR("Invalid light type. Can't generate to XML file.");
	}

	childNode = componentNode.append_child("CastShadow");
	attribute = childNode.append_attribute("value");
	if (m_bCastShadow)
	{
		attribute.set_value("Yes");
	}
	else
	{
		attribute.set_value("No");
	}

	childNode = componentNode.append_child("LightMask");
	attribute = childNode.append_attribute("value");
	attribute.set_value(m_LightMask);

	childNode = componentNode.append_child("LookAt");
	attribute = childNode.append_attribute("x");
	attribute.set_value(m_LookAt.x_);
	attribute = childNode.append_attribute("y");
	attribute.set_value(m_LookAt.y_);
	attribute = childNode.append_attribute("z");
	attribute.set_value(m_LookAt.z_);

	return componentNode;
}