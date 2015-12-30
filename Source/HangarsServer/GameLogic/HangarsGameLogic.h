#ifndef HANGARS_GAME_LOGIC_H
#define HANGARS_GAME_LOGIC_H

#include <EngineStd/GameLogic/BaseGameLogic.h>

class HangarsGameLogic : public BaseGameLogic
{
	URHO3D_OBJECT(HangarsGameLogic,BaseGameLogic)
public:
	HangarsGameLogic(Context* context);
	virtual ~HangarsGameLogic();

public:
	virtual bool VInitialize();
	virtual void VShutdown();

	virtual void VOnUpdate(float timeStep);
	virtual void VChangeState(enum BaseGameState newState);

	virtual bool VLoadGameDelegate(String pLevelData);

protected:
	virtual void VInitializeAllDelegates();		// Register all delegates
	virtual void VDestroyAllDelegates();			// Unsubscribe from all events

	void StartServerDelegate(StringHash eventType, VariantMap& eventData);
	void StopServerDelegate(StringHash eventType, VariantMap& eventData);
	void RestartServerDelegate(StringHash eventType, VariantMap& eventData);
	void PauseServerDelegate(StringHash eventType, VariantMap& eventData);
	void ServerCreatedDelegate(StringHash eventType, VariantMap& eventData);


protected:
	DbConnection* m_pDatabaseConnection;

};

#endif // HANGARS_GAME_LOGIC_H
