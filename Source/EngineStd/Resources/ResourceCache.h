#ifndef RESOURCE_CACHE_H
#define RESOURCE_CACHE_H

namespace SWResourceCache
{
	typedef const Vector<SharedPtr<PackageFile>> PackageFiles;
	typedef SharedPtr<PackageFile> PackageFilePtr;

	Vector<String> Match(ResourceCache* context, const String packageFile, const String pattern);
	String GetResourcePackPath(String path);
	String GetResourcePackName(String path, String ext);
};

#endif
