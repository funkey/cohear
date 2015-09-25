#ifndef COHEAR_RECEIVER_H__
#define COHEAR_RECEIVER_H__

#include <map>
#include <algorithm>
#include <vector>
#include "CallbackDescription.h"
#include "SignalCallbackDescription.h"

namespace chr {

class Receiver {

public:

	typedef std::vector<CallbackDescription*> cds_type;
	typedef std::map<int, cds_type>           groups_type;

	// allow only move construction and assignment, since we exclusively own the 
	// callback descriptors
	Receiver(const Receiver& other) = delete;
	Receiver& operator=(const Receiver& other) = delete;

	Receiver() {}

	Receiver(Receiver&& other) :
		_callbackGroups(other._callbackGroups) {

		other._callbackGroups.clear();
	}

	Receiver& operator=(Receiver&& other) {

		_callbackGroups = other._callbackGroups;
		other._callbackGroups.clear();

		return *this;
	}

	~Receiver() {

		// we own the callback descriptions
		for (groups_type::value_type& groupCds : _callbackGroups)
			for (CallbackDescription* cd : groupCds.second)
				delete cd;
	}

	/**
	 * Register a new callback via a callback description. Ownership of the 
	 * description is transfered to the receiver (the description will be freed 
	 * on desctruction of the receiver).
	 */
	void registerCallback(CallbackDescription* cd, int group = 0) {

		_callbackGroups[group].push_back(cd);
		std::stable_sort(
				_callbackGroups[group].begin(),
				_callbackGroups[group].end(),
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
	void registerCallback(T* obj, int group = 0) {

		registerCallback(
				new SignalCallbackDescription<SignalType, T, Method>(obj),
				group);
	}

	/**
	 * Get descriptions of the callback offered by this receiver.
	 */
	const groups_type& getCallbackDescriptions() { return _callbackGroups; }

private:

	groups_type _callbackGroups;
};

} // namespace chr

#endif // COHEAR_RECEIVER_H__

