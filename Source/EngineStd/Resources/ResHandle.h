#ifndef RESHANDLE_H
#define RESHANDLE_H

#include "Resource.h"

class ResHandle : public Object
{
	URHO3D_OBJECT(ResHandle, Object)

protected:
	SWResource m_Resource;
	SharedPtr<ISWResourceExtraData> m_Extra;

public:
	ResHandle(Context* context, SWResource& resource) : Object(context), m_Resource(resource) { }
	virtual ~ResHandle() { }

	const String GetPath() { return m_Resource.m_Path; }

	SharedPtr<ISWResourceExtraData> GetExtra() { return m_Extra; }
	void SetExtra(SharedPtr<ISWResourceExtraData> extra) { m_Extra = extra; }
};

#endif //RESHANDLE_H
