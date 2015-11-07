#ifndef RESOURCE_H
#define RESOURCE_H

//   IResourceExtraData	- it is a class that can attach extra data to
//   a particular resource. Best example is storing the length and format of a sound file.
//	 Resource can by anything [jpg, mp3, ddt, png, cur , etc] It just take it path to the file.

class ISWResourceExtraData : public Object
{
	URHO3D_OBJECT(ISWResourceExtraData, Object)

public:
	ISWResourceExtraData(Context* context) : Object(context) { }
	virtual String VToString() = 0;
};

class SWResource
{
public:
	SWResource(const String &path);
	String m_Path;
};

#endif
