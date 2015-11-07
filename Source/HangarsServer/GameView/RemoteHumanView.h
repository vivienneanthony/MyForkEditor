#ifndef REMOTE_HUMANS_VIEW_H
#define REMOTE_HUMANS_VIEW_H

class RemoteHumanView : public IGameView
{
	URHO3D_OBJECT(RemoteHumanView,IGameView)
public:
	RemoteHumanView(Context* context);
	virtual ~RemoteHumanView();
};

#endif
