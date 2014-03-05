#ifndef SFC_FWK_BASE_NOTIFIEE_H
#define SFC_FWK_BASE_NOTIFIEE_H

#include "fwkRootNotifiee.h"

namespace fwk
{
/*
Used to define some common attributes and mechanisms for all notifiee classes
*/
template<class Notifier>
class BaseNotifiee : public RootNotifiee 
{
public:
	BaseNotifiee(Notifier *n = 0) : notifier_(n) { if(n) n->notifiee(static_cast<Notifier::Notifiee*>(this)); }
	~BaseNotifiee() { if(notifier_) notifier_->notifieeIs(0); }
	Notifier::PtrConst notifier() const { return notifier_; }
	Notifier::Ptr notifier() { return notifier_; }
	void notifierIs(const Notifier::Ptr n)
	{
		if(notifier_ == n) return;
		if(notifier_) notifier_->notifieeIs(0);
		notifier_ = n;
		if(n) n->notifieeIs(static_cast<Notifier::Notifiee*>(this));
	}
private:
	Notifier::Ptr notifier_;
};
}
//template<class Notifier>
//void BaseNotifiee<Notifier>::notifierIs(const Notifier::Ptr n)
//{
//	if(notifier_ == n) return;
//	if(notifier_) notifier_->notifieeIs(0);
//	notifier_ = n;
//	if(n) n->notifieeIs(static_cast<fwk::Notifier::Notifiee*>(this));
//}

#endif
