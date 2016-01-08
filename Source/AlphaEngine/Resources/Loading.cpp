#include "AlphaEngineStd.h"
#include "Resources/ResourceCache.h"
#include "Loading.h"

ProgressBar::ProgressBar(Context* context, ResourceCache* cache) : Object(context)
{

}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::PreLoad(String path, void(*progressCallback)(int, bool &))
{
	int loaded = 0;

	Vector<String> files = SWResourceCache::Match(g_pApp->GetConstantResCache(), "GameData.pak", path);
	int numFiles = files.Size();
	Vector<String>::Iterator it = files.Begin();

	bool cancel = false;
	for (int i = 0; i < numFiles; ++i)
	{
		String name = (*it);
		m_Cache->GetResource<Resource>(name);
		loaded++;
		if (progressCallback != NULL)
		{
			progressCallback(i * 100 / numFiles, cancel);
		}
		it++;
	}

}

void ProgressBar::PreLoadScene(SharedPtr<File> file, Scene* scene, void(*progressCallback)(int, bool &))
{
	SharedPtr<Engine> engine = g_pApp->GetEngine();
	float loaded = 0.0f;
	bool cancel = false;

	scene->LoadAsyncXML(file);
	while (loaded < 1.0f)
	{
		loaded = scene->GetAsyncProgress();
		engine->Update();
		engine->RunFrame();
		if (progressCallback != NULL)
		{
			progressCallback((int)loaded, cancel);
		}
		else
		{
			ProgressBarCallback((int)(loaded * 100.0f), cancel);
		}
	}

	engine->Update();
	engine->RunFrame();
}

void ProgressBar::ProgressBarCallback(int value, bool& cancel)
{

}
