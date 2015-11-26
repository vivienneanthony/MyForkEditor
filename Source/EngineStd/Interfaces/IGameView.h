#ifndef IGAME_VIEW_H
#define IGAME_VIEW_H

typedef unsigned int GameViewId;
extern const GameViewId INVALID_GAME_VIEW_ID;

enum GameViewType
{
	GameView_Human,
	GameView_Remote,
	GameView_AI,
	GameView_Recorder,
	GameView_Other
};

class IGameView : public Object
{
    friend class BasicGameLogic;

public:
	IGameView(Context* context);
	virtual ~IGameView();

	virtual void VOnAttach(GameViewId vid, GameNodeId aid) = 0;
	virtual bool VOnRestore() = 0;
	virtual bool VOnLostDevice() = 0;
	virtual void VOnUpdate(float timeStep) = 0;
	virtual bool VOnMsgProc(AppMsg msg) = 0;
	virtual void VShutdown() = 0;

	virtual GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;


	inline void VAttachGameAssetManager(GameAssetManager * m_pAttachGameAssetManager, GameAssetFactory *m_pAttachGameAssetFactory)
	{
	    m_pGameAssetManager=m_pAttachGameAssetManager;
	    m_pGameAssetFactory=m_pAttachGameAssetFactory;
	}



protected:
    GameAssetManager * m_pGameAssetManager;                    // Game Asset Manager
    GameAssetFactory * m_pGameAssetFactory;                    // Game Asset Factory

};

#endif
