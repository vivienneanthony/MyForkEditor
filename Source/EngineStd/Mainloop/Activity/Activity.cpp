#include "EngineStd.h"
#include "Activity.h"


// Constructor
Activity::Activity(Context* context) : Object(context)
{
	m_State = UNINITIALIZED;
}

Activity::~Activity()
{
	if (m_pChild)
	{
		m_pChild->VOnAbort();
	}
}


// Removes the child from this Activity.
// This releases ownership of the child to the caller and completely removes it
// from the Activity chain.
StrongActivityPtr Activity::RemoveChild(void)
{
	if (m_pChild)
	{
		StrongActivityPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
		m_pChild.Reset();
		//pChild->SetParent(NULL);
		return pChild;
	}

	return StrongActivityPtr();
}


