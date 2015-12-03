#ifndef REMOTE_HUMANS_VIEW_H
#define REMOTE_HUMANS_VIEW_H

#include "EngineStd/Network/GameView/NetworkGameView.h"

class RemoteHangarsHumanView : public NetworkGameView
{
	URHO3D_OBJECT(RemoteHangarsHumanView, IGameView)
public:
	RemoteHangarsHumanView(Context* context);
	virtual ~RemoteHangarsHumanView();
};

#endif
