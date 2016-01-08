#ifndef ACTIVITY_MANAGER_H
#define ACTIVITY_MANAGER_H

#include "Activity.h"
#include "AlphaEngineApp.h"

typedef SharedPtr<Activity> StrongActivityPtr;
typedef WeakPtr<Activity> WeakActivityPtr;

class ActivityManager : public Object
{
	URHO3D_OBJECT(ActivityManager, Object);

	typedef List<StrongActivityPtr> ActivitiesList;
public:
	// construction
	ActivityManager(Context* context);
	~ActivityManager();

	// interface
	int UpdateActivities(float timeStep);
	WeakActivityPtr AttachActivity(StrongActivityPtr pActivity);
	void AbortAllActivities(bool immediate);

	// accessors
	int GetActivitiesCount() const { return m_ActivitiesList.Size(); }

private:
	void ClearAllActivities(); // call only destructor

protected:
	ActivitiesList m_ActivitiesList;

};

#endif // ACTIVITY_MANAGER_H
