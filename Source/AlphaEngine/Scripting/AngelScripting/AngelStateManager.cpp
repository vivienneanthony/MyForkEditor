#include "AlphaEngineStd.h"
#include "AngelStateManager.h"

#include "Urho3D/AngelScript/APITemplates.h"

bool AngelStateManager::Create()
{
	if (g_pSingleton)
	{
		URHO3D_LOGERROR("Overwriting AngelStateManager singleton");
		SAFE_DELETE(g_pSingleton);
	}

	g_pSingleton = new AngelStateManager();
	if (g_pSingleton)
	{
		return g_pSingleton->VInit();
	}
	return false;
}

void AngelStateManager::Destroy()
{
	assert(g_pSingleton);
	SAFE_DELETE(g_pSingleton);
}

AngelStateManager::AngelStateManager() : IScriptManager(g_pApp->GetContext())
{
	m_pScriptEngine = NULL;
	m_pScriptState = NULL;
}

AngelStateManager::~AngelStateManager()
{

}

bool AngelStateManager::VInit()
{
	m_pScriptState = GetSubsystem<Script>();
	if (m_pScriptState == NULL)
	{
		return false;
	}

	m_pScriptEngine = m_pScriptState->GetScriptEngine();
		
	// register functions
	m_pScriptEngine->RegisterGlobalFunction("void VExecuteFile(String)", asMETHOD(AngelStateManager, VExecuteFile), asCALL_THISCALL_ASGLOBAL, g_pSingleton);
	m_pScriptEngine->RegisterGlobalFunction("void VExecuteString(String)", asMETHOD(AngelStateManager, VExecuteString), asCALL_THISCALL_ASGLOBAL, g_pSingleton);

	return true;
}

void AngelStateManager::VExecuteFile(String resource)
{
	//m_pScriptEngine->
	
}

void AngelStateManager::VExecuteString(String string)
{

}

asIScriptEngine* AngelStateManager::GetScriptEngine()
{
	return m_pScriptEngine;
}

Script* AngelStateManager::GetScriptState()
{
	return m_pScriptState;
}

void AngelStateManager::SetError(int errorNum)
{

}

void AngelStateManager::ClearStack()
{

}
