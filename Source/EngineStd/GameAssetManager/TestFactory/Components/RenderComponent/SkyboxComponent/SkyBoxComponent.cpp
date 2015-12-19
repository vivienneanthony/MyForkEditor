#include "EngineStd.h"
#include "SkyBoxComponent.h"


const String SkyBoxComponent::g_Name = String("SkyBoxComponent");

unsigned int SkyBoxComponent::m_SkyboxCount = 0;

SkyBoxComponent::SkyBoxComponent(Context* context) : MainComponent(context)
{
	m_bIsApplyMaterial = false;
}

SkyBoxComponent::SkyBoxComponent()
{
	m_bIsApplyMaterial = false;
}

SkyBoxComponent::~SkyBoxComponent()
{

}

// mainComponent interface
bool SkyBoxComponent::VInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("Model");
	if (node)
	{
		m_SkyboxModel = node.attribute("path").as_string();
	}

	node = pData->child("Material");
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

	m_pNodeSkybox = GetNode()->CreateComponent<Skybox>(m_CreateMode, MainComponent::GetUniqueIdForURHO(GetNode()->GetID(), g_Name, m_SkyboxCount));
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