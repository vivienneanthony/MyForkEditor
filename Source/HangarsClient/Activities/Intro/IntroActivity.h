#pragma once

#include "EngineStd/Mainloop/Activity/Activity.h"

class IntroActivity : public Activity
{
	URHO3D_OBJECT(IntroActivity, Activity);
public:
	IntroActivity(Context* context);
	virtual ~IntroActivity();

	virtual void VOnInit();
	virtual void VOnUpdate(float timeStep);
	virtual void VOnSuccess();
	
protected:
	float m_IntroTime;							// How long show intro
	float m_TotalElapsedTime;
	SharedPtr<Text> m_Text;
};

