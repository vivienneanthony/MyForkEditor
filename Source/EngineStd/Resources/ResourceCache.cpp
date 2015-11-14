#include "EngineStd.h"
#include "ResourceCache.h"
#include "Utility/String.h"


Vector<String> SWResourceCache::Match(ResourceCache* resCache, const String packageFile, const String pattern)
{	
	Vector<String> matchingNames;
	
	if (resCache == nullptr)
	{
		return matchingNames;
	}

	PackageFilePtr package;
	PackageFiles& packageFiles = resCache->GetPackageFiles();
	PackageFiles::ConstIterator it = packageFiles.Begin();
	for (; it != packageFiles.End(); it++)
	{

		String name = GetResourcePackPath(it->Get()->GetName());
		if (packageFile == name)
		{
			package = it->Get();
			break;
		}
	}

	const Vector<String> files = it->Get()->GetEntryNames();
	
	for (Vector<String>::ConstIterator it = files.Begin(); it != files.End(); it++)
	{
		String fileName = it->CString();
		if (WildcardMatch(pattern.CString(), fileName.CString()))
		{
			matchingNames.Push(fileName);
		}
	}
	return matchingNames;
}

String SWResourceCache::GetResourcePackPath(String path)
{
	String pattern = ".pak";
	int size = path .Length() - 5;
	for (; size >= 0; size--)
	{
		char letter = path[size];
		// ascii code 47 = */* (backslash)
		// ascii code 92 = *\*
		if (static_cast<int>(letter) == 47 || static_cast<int>(letter) == 92)
		{
			size++;
			break;
		}
	}
	return path.Substring(size);
}

String SWResourceCache::GetResourcePackName(String path, String ext)
{
	int size = path.Length() - 5;
	for (; size >= 0; size--)
	{
		char letter = path[size];
		// ascii code 47 = */* (backslash)
		// ascii code 92 = *\*
		if (static_cast<int>(letter) == 47 || static_cast<int>(letter) == 92)
		{
			size++;
			break;
		}
	}
	String substring = path.Substring(size);
	substring.Erase(substring.Length() - ext.Length(), ext.Length());
	return substring;
}