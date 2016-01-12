#include "AlphaEngineStd.h"
#include "SkyBoxComponent.h"


const String SkyBoxComponent::g_Name = String("SkyBoxComponent");

unsigned int SkyBoxComponent::m_SkyboxCount = 0;

SkyBoxComponent::SkyBoxComponent(Context* context) : MainComponent(context)
,m_SkyboxModel(String::EMPTY)
,m_SkyboxMaterial(String::EMPTY)
,m_pNodeSkybox(NULL)

{
	m_bIsApplyMaterial = false;
}

SkyBoxComponent::SkyBoxComponent()
{
	m_bIsApplyMaterial = false;

	// rest of defaults
	m_SkyboxModel.Clear();
    m_SkyboxMaterial.Clear();
    m_pNodeSkybox = NULL;
}

SkyBoxComponent::~SkyBoxComponent()
{

}

// mainComponent interface
bool SkyBoxComponent::VInit(pugi::xml_node pData)
{
	pugi::xml_node node = pData.child("Model");
	if (node)
	{
		m_SkyboxModel = node.attribute("path").as_string();
	}

	node = pData.child("Material");
	if (node)
	{
		m_SkyboxMaterial = node.attribute("path").as_string();
		m_bIsApplyMaterial = (m_SkyboxMaterial != "") ? true : false;
	}

	return true;
}

void SkyBoxComponent::VPostInit(void)
{
	m_SkyboxCount++;

	m_pNodeSkybox = GetNode()->CreateComponent<Skybox>(m_CreateMode, GetUniques::GetUniqueIdForURHO(GetNode()->GetID(), g_Name, m_SkyboxCount));
	if (!m_pNodeSkybox)
	{
		URHO3D_LOGDEBUG("Failed to create Urho3D::Skybox component in SkyBoxComponent to Node with ID " + String(GetNode()->GetID()));
	}

	ResourceCache* cache = g_pApp->GetConstantResCache();

	m_pNodeSkybox->SetModel(cache->GetResource<Model>(m_SkyboxModel));

	if (m_bIsApplyMaterial)
	{
		m_pNodeSkybox->SetMaterial(cache->GetResource<Material>(m_SkyboxMaterial));
	}
	else
	{
		m_pNodeSkybox->ApplyMaterialList();
	}
}

void SkyBoxComponent::VUpdate(float timeStep)
{

}

void SkyBoxComponent::VOnChanged(void)
{
	if (m_pNodeSkybox)
	{
		ResourceCache* cache = g_pApp->GetConstantResCache();

		m_pNodeSkybox->SetModel(cache->GetResource<Model>(m_SkyboxModel));

		if (m_bIsApplyMaterial)
		{
			m_pNodeSkybox->SetMaterial(cache->GetResource<Material>(m_SkyboxMaterial));
		}
		else
		{
			m_pNodeSkybox->ApplyMaterialList();
		}
	}
}

pugi::xml_node SkyBoxComponent::VGenerateXML(pugi::xml_node root)
{
	pugi::xml_node componentNode = root.append_child(VGetName().CString());
	pugi::xml_attribute componentAttribute = componentNode.append_attribute("name");
	componentAttribute.set_value(VGetName().CString());

	pugi::xml_node childNode = componentNode.append_child("Model");
	pugi::xml_attribute attribute = childNode.append_attribute("path");
	attribute.set_value(m_SkyboxModel.CString());

	childNode = componentNode.append_child("Material");
	attribute = childNode.append_attribute("path");
	attribute.set_value(m_SkyboxMaterial.CString());

	return componentNode;
}
