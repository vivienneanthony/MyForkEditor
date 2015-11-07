#ifndef HANGARS_HUMAN_VIEW_H
#define HANGARS_HUMAN_VIEW_H

#include <EngineStd/UserInterface/HumanView/HumanView.h>

class HangarsHumanView : public HumanView
{
	URHO3D_OBJECT(HangarsHumanView, HumanView)
public:
	HangarsHumanView(Context* context, Renderer* renderer);
	virtual ~HangarsHumanView();
};

#endif
