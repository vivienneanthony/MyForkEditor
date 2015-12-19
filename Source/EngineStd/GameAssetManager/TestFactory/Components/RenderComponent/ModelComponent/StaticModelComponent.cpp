#include "EngineStd.h"
#include "StaticModelComponent.h"


const String StaticModelComponent::g_Name = String("StaticModelComponent");

unsigned int StaticModelComponent::m_ModelCount = 0;

StaticModelComponent::StaticModelComponent(Context* context) : MainComponent(context)
{
	m_bIsApplyMaterial = false;
}

StaticModelComponent::StaticModelComponent()
{
	m_bIsApplyMaterial = false;
}

StaticModelComponent::~StaticModelComponent()
{
	m_ModelCount--;
}

// mainComponent interface
bool StaticModelComponent::VInit(pugi::xml_node* pData)
{
	pugi::xml_node node = pData->child("Model");
	if (node)
	{
		m_ModelResource = node.attribute("path").as_string();
	}
	
	node = pData->child("Material");
	if (node)
	{
		m_MaterialResource = node.attribute("path").as_string();
		m_bIsApplyMaterial = (m_MaterialResource != "") ? true : false;
	}

	node = pData->child("CastShadow");
	if (node)
	{
		String value = node.attribute("value").as_string();
		m_bCastShadow = (value != "yes") ? true : false;
	}

	node = pData->child("LightMask");
	if (node)
	{
		m_LightMask = node.attribute("value").as_uint();
	}

	return true;
}

void StaticModelComponent::VPostInit(void)
{
	m_ModelCount++;

	m_pModel = GetNode()->CreateComponent<StaticModel>(m_CreateMode, MainComponent::GetUniqueIdForURHO(GetNode()->GetID(), g_Name, m_ModelCount));
	if (!m_pModel)
	{
		URHO3D_LOGDEBUG("Failed to create Urho3D::StaticModel component in StaticModelComponent to Node with ID " + String(GetNode()->GetID()));
	}

	ResourceCache* cache = g_pApp->GetConstantResCache();

	m_pModel->SetModel(cache->GetResource<Model>(m_ModelResource));

	m_pModel->SetCastShadows(m_bCastShadow);
	m_pModel->SetLightMask(m_LightMask);

	if (m_bIsApplyMaterial)
	{
		m_pModel->SetMaterial(cache->GetResource<Material>(m_MaterialResource));
	}
	else
	{
		m_pModel->ApplyMaterialList();
	}
}

void StaticModelComponent::VUpdate(float timeStep)
{

}

void StaticModelComponent::VOnChanged(void)
{
	if (m_pModel)
	{
		ResourceCache* cache = g_pApp->GetConstantResCache();

		m_pModel->SetModel(cache->GetResource<Model>(m_ModelResource));

		m_pModel->SetCastShadows(m_bCastShadow);
		m_pModel->SetLightMask(m_LightMask);

		if (m_bIsApplyMaterial)
		{
			m_pModel->SetMaterial(cache->GetResource<Material>(m_MaterialResource));
		}
		else
		{
			m_pModel->ApplyMaterialList();
		}
	}
}