#include "EngineStd.h"
#include "ActivityManager.h"

ActivityManager::ActivityManager(Context* context) : Object(context)
{


}

ActivityManager::~ActivityManager()
{

}

int ActivityManager::UpdateActivities(float timeStep)
{
	unsigned short int succesCount = 0;
	unsigned short int failCount = 0;

	ActivitiesList::Iterator it = m_ActivitiesList.Begin();
	while (it != m_ActivitiesList.End())
	{
		// grab the next Activity
		StrongActivityPtr pCurrActivity = (*it);

		// save the iterator and increment old one in case we need to remove
		// this Activity from the list
		ActivitiesList::Iterator thisIt = it;
		++it;

		// Activity is unitinialized, so initialize it
		if (pCurrActivity->GetState() == Activity::UNINITIALIZED)
		{
			pCurrActivity->VOnInit();
		}

		// give the Activity an update tick if itš running
		if (pCurrActivity->GetState() == Activity::RUNNING)
		{
			pCurrActivity->VOnUpdate(timeStep);
		}

		// check to see if the Activity dead
		if (pCurrActivity->IsDead())
		{
			// run the appropriate exit function
			switch (pCurrActivity->GetState())
			{
			case Activity::SUCCEEDED:
				{
					pCurrActivity->VOnSuccess();
					StrongActivityPtr pChild = pCurrActivity->RemoveChild();
					if (pChild)
					{
						AttachActivity(pChild);
					}
					else
					{
						++succesCount; // only counts if whole chain complete
					}
					break;
				}
			case Activity::FAILED:
				{
					pCurrActivity->VOnFail();
					++failCount;
					break;
				}
			case Activity::ABORTED:
				{
					pCurrActivity->VOnAbort();
					++failCount;
					break;
				}
			}

			// remove the Activity and destroy it
			m_ActivitiesList.Erase(thisIt);
		}
	}
	return ((succesCount << 16) | failCount);
}

//---------------------------------------------------------------------------------------------------------------------
// Attaches the Activity to the Activities list so it can be run on the next update.
//---------------------------------------------------------------------------------------------------------------------
WeakActivityPtr ActivityManager::AttachActivity(StrongActivityPtr pActivity)
{
	m_ActivitiesList.PushFront(pActivity);
	return WeakActivityPtr(pActivity);
}

//---------------------------------------------------------------------------------------------------------------------
// Clears all Activities (and DOESN'T run any exit code)
//---------------------------------------------------------------------------------------------------------------------
void ActivityManager::ClearAllActivities(void)
{
	m_ActivitiesList.Clear();
}

//---------------------------------------------------------------------------------------------------------------------
// Aborts all Activities.  If immediate == true, it immediately calls each ones OnAbort() function and destroys all
// the Activities.
//---------------------------------------------------------------------------------------------------------------------
void ActivityManager::AbortAllActivities(bool immediate)
{
	ActivitiesList::Iterator it = m_ActivitiesList.Begin();
	while (it != m_ActivitiesList.End())
	{
		ActivitiesList::Iterator tempIt = it;
		++it;

		StrongActivityPtr pActivity = *tempIt;
		if (pActivity->IsAlive())
		{
			pActivity->SetState(Activity::ABORTED);
			if (immediate)
			{
				pActivity->VOnAbort();
				m_ActivitiesList.Erase(tempIt);
			}
		}
	}
}


