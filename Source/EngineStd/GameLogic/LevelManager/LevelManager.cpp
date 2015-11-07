#include "EngineStd.h"
#include "LevelManager.h"

LevelManager::LevelManager(Context* context) : Object(context)
{

}

LevelManager::~LevelManager()
{


}

bool LevelManager::Initialize(Vector<String> &levels)
{
	Vector<String>::Iterator i = levels.Begin();
	while (i != levels.End())
	{
		m_Levels.Push(*i);
		++i;
	}
	return true;
}

void LevelManager::Shutdown()
{


}

