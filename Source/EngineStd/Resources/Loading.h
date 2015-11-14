#ifndef LOADING_H
#define LOADING_H

class ProgressBar : public Object
{
	URHO3D_OBJECT(ProgressBar, Object)
public:
	ProgressBar(Context* context, ResourceCache* cache);
	~ProgressBar();

	void PreLoad(String path, void(*progressCallback)(int, bool &));
	void PreLoadScene(SharedPtr<File> file, Scene* scene, void(*progressCallback)(int, bool &));

	void ProgressBarCallback(int value, bool& cancel);


protected:
	ResourceCache* m_Cache;
	String m_Pattern;
};

#endif // LOADING_H
