
#pragma once

class XMLResourceLoader : public Object
{
	URHO3D_OBJECT(XMLResourceLoader, Object)
public:
	static pugi::xml_document* LoadXMLDocument(String resource);

};