#ifndef COHEAR_RECEIVER_H__
#define COHEAR_RECEIVER_H__

#include <algorithm>
#include "CallbackDescription.h"
#include "SignalCallbackDescription.h"
#include "PassThroughCallbackDescription.h"
#include "FilterCallbackDescription.h"

namespace chr {

// forward declaration
class PassThroughSlot;

class Receiver {

public:

	~Receiver() {

		// we own the callback descriptions
		for (CallbackDescription* cd : _cds)
			delete cd;
	}

	/**
	 * Register a method as a callback:
	 *
	 *   registerCallback<SignalType, Class, &Class::method>(&instance);
	 */
	template <typename SignalType, class T, void (T::*Method)(SignalType&)>
	void registerCallback(T* obj) {

		registerCallback(
				new SignalCallbackDescription<SignalType, T, Method>(obj));
	}

	template <typename SignalType>
	void registerPassThroughCallback(PassThroughSlot* targetSlot) {

		registerCallback(
				new PassThroughCallbackDescription<SignalType>(targetSlot));
	}

	template <typename SignalType>
	void registerFilterCallback(FilterSlot* targetSlot) {

		registerCallback(
				new FilterCallbackDescription<SignalType>(targetSlot));
	}

	/**
	 * Get descriptions of the callback offered by this receiver.
	 */
	const std::vector<CallbackDescription*>& getCallbackDescriptions() { return _cds; }

private:

	/**
	 * Register a new callback via a callback description. Ownership of the 
	 * description is transfered to the receiver (the description will be freed 
	 * on desctruction of the receiver).
	 */
	void registerCallback(CallbackDescription* cd) {

		_cds.push_back(cd);
		std::stable_sort(
				_cds.begin(),
				_cds.end(),
				[](CallbackDescription* a, CallbackDescription* b){
						return a->getPrecedence() > b->getPrecedence();
				}
		);
	}

	std::vector<CallbackDescription*> _cds;
};

} // namespace chr

#endif // COHEAR_RECEIVER_H__

