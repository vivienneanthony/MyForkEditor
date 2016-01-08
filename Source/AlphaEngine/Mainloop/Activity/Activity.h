#ifndef ACTIVITY_H
#define ACTIVITY_H

class Activity;
typedef SharedPtr<Activity> StrongActivityPtr;
typedef WeakPtr<Activity> WeakActivityPtr;

//---------------------------------------------------------------------------------------------------------------------
// Activity class

// Activity is able to have 3 states: running, paused, suceeded, aborted.
//		- Success means the activity completed successfully. If the activity has a child, it will be attached to
//		  the activity manager, but previous acitvity will be deleted.
//		- Failure means the activity started but failed in some way. If the activity has a child, it will be
//		  aborted. No sense, to start child activity, if parent'activity was failed.
//		- Aborted activities are activities that are canceled while not submitted to the Activity mgr. Depending
//		  on the circumstances, they may or may not have gotten an OnInit() call. For example, a activity can
//		  spawn another activities and call AttachToParent() on itself. If the new activity fails, the child will
//		  get an Abort() call on it, even though its status is RUNNING.
//---------------------------------------------------------------------------------------------------------------------

class Activity : public Object
{
	URHO3D_OBJECT(Activity, Object);

	friend class ActivityManager;

public:
	enum State
	{
		// Activities that are neither dead nor alive
		UNINITIALIZED = 0,  // created but not running
		REMOVED,  // removed from the activity list but not destroyed; this can happen when a Activity that is already running is parented to another Activity

		// Living Activities
		RUNNING,  // initialized and running
		PAUSED,  // initialized but paused

		// Dead Activities
		SUCCEEDED,  // completed successfully
		FAILED,  // failed to complete
		ABORTED,  // aborted; may not have started
	};

public:
	// construction
	Activity(Context* m_pContext);
	virtual ~Activity();

public:
	// Functions for ending the Activity.
	inline void Succeed();
	inline void Fail();

	// pause
	inline void Pause();
	inline void UnPause();

	// accessors
	State GetState() const { return m_State; }
	bool IsAlive() const { return (m_State == RUNNING || m_State == PAUSED); }
	bool IsDead() const { return (m_State == SUCCEEDED || m_State == FAILED || m_State == ABORTED); }
	bool IsRemoved() const { return (m_State == REMOVED); }
	bool IsPaused() const { return m_State == PAUSED; }

	// child functions
	inline void AttachChild(StrongActivityPtr pChild);
	StrongActivityPtr RemoveChild(void);  // releases ownership of the child
	StrongActivityPtr PeekChild(void) { return m_pChild; }  // doesn't release ownership of the child

protected:
	// interface; these functions should be overridden by the subclass as needed
	virtual void VOnInit() { m_State = RUNNING; }  // called during the first update; responsible for setting the initial state (typically RUNNING)
	virtual void VOnUpdate(float timeStep) = 0;  // called every frame
	virtual void VOnSuccess() { }  // called if the Activity succeeds (see below)
	virtual void VOnFail() { }  // called if the Activity fails (see below)
	virtual void VOnAbort() { }  // called if the Activity is aborted (see below)

private:
	void SetState(State newState) { m_State = newState; }

private:
	State m_State;  // the current state of the Activity
	StrongActivityPtr m_pChild;  // the child Activity, if any

};


//---------------------------------------------------------------------------------------------------------------------
// Inline function definitions
//---------------------------------------------------------------------------------------------------------------------
inline void Activity::Succeed()
{
	assert(m_State == RUNNING || m_State == PAUSED);
	m_State = SUCCEEDED;
}

inline void Activity::Fail()
{
	assert(m_State == RUNNING || m_State == PAUSED);
	m_State = FAILED;
}

inline void Activity::AttachChild(StrongActivityPtr pChild)
{
	if (m_pChild)
	{
		m_pChild->AttachChild(pChild);
	}
	else
	{
		m_pChild = pChild;
	}
}

inline void Activity::Pause()
{
	if (m_State == RUNNING)
	{
		m_State = PAUSED;
	}
	else
	{
		URHO3D_LOGERROR("Attempting to pause a activity that isn't running");
	}
}

inline void Activity::UnPause()
{
	if (m_State == PAUSED)
	{
		m_State = RUNNING;
	}
	else
	{
		URHO3D_LOGERROR("Attempting to unpause a activity that isn't paused");
	}
}

#endif // ACTIVITY_H
