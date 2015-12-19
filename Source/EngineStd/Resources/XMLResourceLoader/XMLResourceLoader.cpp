#include "EngineStd.h"
#include "XMLResourceLoader.h"


pugi::xml_document* XMLResourceLoader::LoadXMLDocument(String resource)
{
	ResourceCache* cache = g_pApp->GetConstantResCache();

	XMLFile* file = cache->GetResource<XMLFile>(resource);
	if (file)
	{
		pugi::xml_document* document = file->GetDocument();
		if (document)
		{
			return document;
		}
	}
	else
	{
		URHO3D_LOGERROR("Failed to load XMLResource");
	}

	return NULL;
}