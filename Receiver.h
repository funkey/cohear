#ifndef SR_RECEIVER_H__
#define SR_RECEIVER_H__

#include "CallbackDescription.h"
#include "SignalCallbackDescription.h"
#include "PassThroughCallbackDescription.h"

namespace sr {

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

		registerCallback(sr::SignalCallbackDescription<SignalType>::template Create<T, Method>(obj));
	}

	template <typename SignalType>
	void registerPassThroughCallback(PassThroughSlot* targetSlot) {

		registerCallback(new sr::PassThroughCallbackDescription<SignalType>(targetSlot));
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
	}

	std::vector<CallbackDescription*> _cds;
};

} // namespace sr

#endif // SR_RECEIVER_H__

