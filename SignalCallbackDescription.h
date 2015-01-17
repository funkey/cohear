#ifndef SR_SIGNAL_CALLBACK_DESCRIPTION_H__
#define SR_SIGNAL_CALLBACK_DESCRIPTION_H__

#include "Delegate.h"
#include "CallbackDescription.h"

namespace chr {

template <typename SignalType>
class SignalCallbackDescription : public CallbackDescription {

public:

	template <class T, void (T::*Method)(SignalType&)>
	static SignalCallbackDescription<SignalType>* Create(T* obj) {

		return new SignalCallbackDescription<SignalType>(
				Delegate<SignalType>::template from_method<T, Method>(obj),
				obj,
				typeid(Method)
		);
	}

	/**
	 * Get an untyped pointer to the delegate that invokes the callback. Used by 
	 * get-delegate-strategies (which know what they do). Use at own risk.
	 */
	virtual void* getDelegate() override final { return &_delegate; }

private:

	SignalCallbackDescription(
			Delegate<SignalType>&& delegate,
			void* obj,
			std::type_index functionTypeIndex) :
		CallbackDescription(typeid(SignalType), functionTypeIndex, obj),
		_delegate(delegate) {}

	Delegate<SignalType> _delegate;
};

} // namespace chr

#endif // SR_SIGNAL_CALLBACK_DESCRIPTION_H__

