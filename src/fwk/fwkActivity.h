#ifndef SFC_FWK+ACTIVITY_H
#define SFC_FWK_ACTIVITY_H

#include "FwkNamedInterface.h"
#include "FwkRootNotifiee.h"

namespace Fwk
{
/*
An object that represents an allocation of processing capacity that
executes queued notifications. It executes each notification processing to completion,
being executed by a thread provided by the operating system. That is, it is a wrapper
and layer of indirection from the operating system processing resource.
An activity is instantiated to handle processing in response to notifications that
require significant processing time thus is too expensive to be performed directly in the 
notification handler. In this case the associated notifiee is enqueued for
its activity and the notifier is allowed to continue immediately.
In typical use, the object implementation creates the activities it requires on its
initialization or on demand and retains each activity for as long as it is useful. It
does not create and destroy an activity on each callback. Instead, an activity serves
as an allocation of processing resources. And, the cost of creating and deleting
activities is amortized over many callbacks,
*/
class Activity : public NamedInterface 
{
public:
	/*
	Each manager can be allocated some number of threads of control as well as a portion of the processor. 
	The number of threads limits the number of activities that can be in execution at one time. 
	Once this limit is reached, subsequent ready activities need to wait for a thread of control
	to become available.
	Activities that are performing separate tasks with different timing should be implemented by a separate manager.
	The manager implements a ready queue, a pool of threads and a pool of activities.
	When an activity becomes ready because a notifiee is queued, it is enqueued in the
	ready queue. The ready queue is ordered by activity priority (as computed by the
	selected scheduling algorithm). When a thread from the thread pool is available
	to execute an activity, it dequeues the highest priority activity in the ready queue,
	and calls its RootNotifiee::handleNotification function. If there is no ready
	activity, the thread suspends waiting for a ready activity.
	*/
	class Manager
	{
	public:
		/*
		A new activity is instantiated by a call to the Activity::Manager.
		*/
		Activity::Ptr activityIs(Name n);
	};
	enum Status { null, ready, executing, delaying };
	Status status() const { return status_; }

	/*
	When a notifiee is enqueued as a scheduled notifiee by writing lastNotifieeIs, the
	activity is scheduled to execute, if it is not already executing. An activity is executed
	by assigning a thread to execute it. The thread executes each notifiee enqueued for
	the activity, dequeuing each notifiee and calling the handleNotification on each
	notifiee.
	An activity is designed to execute notifiees sequentially so there cannot be two
	callbacks by the same activity at the same time.
	*/
	void lastNotifieeIs(RootNotifiee *n);

private:
	enum Status status_;
};
}

#endif
