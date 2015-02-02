#ifndef COHEAR_RECEIVER_H__
#define COHEAR_RECEIVER_H__

#include <algorithm>
#include "CallbackDescription.h"
#include "SignalCallbackDescription.h"

namespace chr {

class Receiver {

public:

	~Receiver() {

		// we own the callback descriptions
		for (CallbackDescription* cd : _cds)
			delete cd;
	}

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

	/**
	 * Register a method as a callback. Usage:
	 *
	 *   registerCallback<SignalType, Class, &Class::method>(&instance);
	 *
	 * This is a convenience wrapper for creating and registering a 
	 * SignalCallbackDescription.
	 */
	template <typename SignalType, class T, void (T::*Method)(SignalType&)>
	void registerCallback(T* obj) {

		registerCallback(
				new SignalCallbackDescription<SignalType, T, Method>(obj));
	}

	/**
	 * Get descriptions of the callback offered by this receiver.
	 */
	const std::vector<CallbackDescription*>& getCallbackDescriptions() { return _cds; }

private:

	std::vector<CallbackDescription*> _cds;
};

} // namespace chr

#endif // COHEAR_RECEIVER_H__

