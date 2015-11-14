#include "HangarsServerStd.h"

#include "EngineStd/Mainloop/Activity/ActivityManager.h"

#include "MainMenuView.h"
#include "UserInterface/MainMenuUI.h"
#include "Activities/Intro/IntroActivity.h"

MainMenuView::MainMenuView(Context* context, Renderer* renderer, bool intro) : HumanView(context, renderer)
{
	m_pMainMenu = SharedPtr<IScreenElement>(new MainMenuUI(context));
	VPushElement(m_pMainMenu);
	m_pMainMenu->VSetVisible(false);

	VInitializeAllDelegates();

	if (intro)
	{
		StrongActivityPtr intro = SharedPtr<Activity>(new IntroActivity(context_));
		m_pActivityManager->AttachActivity(intro);
	}
	else
	{
		SendEvent("Finish_Introduction_Part");
	}

}

MainMenuView::~MainMenuView()
{


}

void  MainMenuView::VInitializeAllDelegates()
{
	HumanView::VInitializeAllDelegates();
	SubscribeToEvent("Finish_Introduction_Part", URHO3D_HANDLER(MainMenuView, FinishIntroductionPartDelegate));

}

void  MainMenuView::VDestroyAllDelegates()
{
	HumanView::VDestroyAllDelegates();
}

void  MainMenuView::FinishIntroductionPartDelegate(StringHash eventType, VariantMap& eventData)
{
	if (m_pMainMenu)
	{
		m_pMainMenu->VOnRestore();
	}

	m_pMainMenu->VSetVisible(true);

	UnsubscribeFromEvent("Finish_Introduction_Part");
}

