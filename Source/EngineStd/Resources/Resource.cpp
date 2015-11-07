#include "EngineStd.h"
#include "Resource.h"

SWResource::SWResource(const String &path)
{
	m_Path = path;
	m_Path.ToLower();
}
