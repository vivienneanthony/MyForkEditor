#include "AlphaEditorStd.h"

#include "EditorGameLogic.h"


EditorGameLogic::EditorGameLogic(Context* context) : BaseGameLogic(context)
{


}

EditorGameLogic::~EditorGameLogic()
{

}

bool EditorGameLogic::VInitialize()
{
    BaseGameLogic::VInitialize();

    return true;
}

void EditorGameLogic::VShutdown()
{


    // Add Hoc Shutdown Letting AngelScript remove everything
    BaseGameLogic::VShutdown();
}

void EditorGameLogic::VChangeState(enum BaseGameState newState)
{
    BaseGameLogic::VChangeState(newState);

	if (m_State == BGS_WaitingForPlayer)
	{
		VSetProxy();

		g_pApp->AttachAsClient();
	}

}

bool EditorGameLogic::VLoadGameDelegate(String pLevelData)
{
    return true;
}








