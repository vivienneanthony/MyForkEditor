#pragma once

class IScriptManager : public Urho3D::Object
{
public:
	IScriptManager(Context* context);
	virtual ~IScriptManager();
	virtual bool VInit() = 0;
	virtual void VExecuteFile(String resource) = 0;
	virtual void VExecuteString(String string) = 0;
};
