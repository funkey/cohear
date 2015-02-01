#ifndef COHEAR_SIGNAL_CALLBACK_DESCRIPTION_H__
#define COHEAR_SIGNAL_CALLBACK_DESCRIPTION_H__

#include "Delegate.h"
#include "CallbackDescription.h"
#include "detail/SignalTraits.h"

namespace chr {

template <typename SignalType, class T, void (T::*Method)(SignalType&)>
class SignalCallbackDescription : public CallbackDescription {

public:

	SignalCallbackDescription(T* obj) :
		CallbackDescription(typeid(SignalType), typeid(Method), obj),
		_delegate(Delegate<SignalType>::template from_method<T, Method>(obj)) {

		// the more specific a signal, the higher the precedence of the callback
		setPrecedence(SignalTraits<SignalType>::specificity);
	}

	/**
	 * Get an untyped pointer to the delegate that invokes the callback. Used by 
	 * get-delegate-strategies (which know what they do). Use at own risk.
	 */
	void* notifySlotConnect(detail::SlotBase* const) override final { return &_delegate; }

private:

	Delegate<SignalType> _delegate;
};

} // namespace chr

#endif // COHEAR_SIGNAL_CALLBACK_DESCRIPTION_H__

