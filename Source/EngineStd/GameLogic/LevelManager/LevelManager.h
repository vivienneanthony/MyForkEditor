#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

typedef String Level;
typedef Vector<Level> Levels;

class LevelManager : public Object
{
	URHO3D_OBJECT(LevelManager, Object)
public:
	// Constructors
	LevelManager(Context* context);
	virtual ~LevelManager();

	bool Initialize(Vector<String>& levels);
	void Shutdown();


	// Getters/Setters
	const Levels& GetLevels() const { return m_Levels; }
	const int GetCurrentLevel() const { return m_CurrentLevel; }

protected:
	Levels m_Levels;
	int m_CurrentLevel;
};

#endif
