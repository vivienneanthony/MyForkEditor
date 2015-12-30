#pragma once

class AngelStateManager : public IScriptManager
{
	URHO3D_OBJECT(AngelStateManager, IScriptManager)	
	static AngelStateManager* g_pSingleton;
public:
	// Singleton functions
	static bool Create();
	static void Destroy();
	static AngelStateManager* Get() { assert(g_pSingleton); return g_pSingleton; }

	// IScriptManager interface
	virtual bool VInit();
	virtual void VExecuteFile(String resource);
	virtual void VExecuteString(String string);

	asIScriptEngine* GetScriptEngine();
	Script* GetScriptState();

private:
	void SetError(int errorNum);
	void ClearStack();

	// private constructor & destructor; call the static Create() and Destroy() functions instead
	explicit AngelStateManager();
	virtual ~AngelStateManager();

private:
	Script* m_pScriptState;
	asIScriptEngine* m_pScriptEngine;
	String m_LastError;

};
