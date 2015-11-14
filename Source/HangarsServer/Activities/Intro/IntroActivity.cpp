#include "HangarsServerStd.h"
#include "EngineStd/Mainloop/Activity/Activity.h"
#include "EngineStd/GameLogic/BaseGameLogic.h"

#include "GameView/MainMenuView.h"
#include "IntroActivity.h"


IntroActivity::IntroActivity(Context* context) : Activity(context)
{


}

IntroActivity::~IntroActivity()
{


}

void IntroActivity::VOnInit()
{
	Activity::VOnInit();
	m_TotalElapsedTime = 0.0f;
	m_IntroTime = 3.0f;

	ResourceCache* resCache = g_pApp->GetConstantResCache();
	
	// Construct new Text object
	m_Text = SharedPtr<Text>(new Text(context_));

	// Set String to display
	m_Text->SetText("GarageProduction Inc.");
	
	// Set font and text color
	m_Text->SetFont(resCache->GetResource<Font>("UI/Fonts/scarabeo.ttf"), 60);
	m_Text->SetColor(Color(0.6f, 0.6f, 0.6f));

	// Align Text center-screen
	m_Text->SetHorizontalAlignment(HA_CENTER);
	m_Text->SetVerticalAlignment(VA_CENTER);
	
	// Add Text instance to the UI root element
	GetSubsystem<UI>()->GetRoot()->AddChild(m_Text);
}

void IntroActivity::VOnUpdate(float timeStep)
{
	m_TotalElapsedTime += timeStep;
	if (m_TotalElapsedTime > m_IntroTime)
	{
		m_Text->SetEnabled(false);
		Succeed();
	}
}

void IntroActivity::VOnSuccess()
{
	ResourceCache* resCache = g_pApp->GetConstantResCache();
	m_Text->SetText("");
	GetSubsystem<UI>()->GetRoot()->RemoveChild(m_Text);
	SendEvent("Finish_Introduction_Part");
}


