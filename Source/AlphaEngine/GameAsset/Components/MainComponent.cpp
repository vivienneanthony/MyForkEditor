#include "AlphaEngineStd.h"
#include "MainComponent.h"

const String MainComponent::g_Name = String("MainComponent");

MainComponent::MainComponent(Context* context) : LogicComponent(context)
, m_CreateMode(Urho3D::CreateMode::LOCAL)
{
    // Kept but not needed
	m_CreateMode = Urho3D::CreateMode::LOCAL;

}

MainComponent::MainComponent() : LogicComponent(g_pApp->GetContext())
,m_CreateMode(Urho3D::CreateMode::LOCAL)
{
    // Kept but not needed
	m_CreateMode = Urho3D::CreateMode::LOCAL;
}

MainComponent::~MainComponent()
{

}

void MainComponent::FixedUpdate(float timeStep)
{
	// Call overrided updates
	VUpdate(timeStep);
}
