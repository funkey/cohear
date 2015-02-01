#ifndef COHEAR_CALLBACK_DESCRIPTION_H__
#define COHEAR_CALLBACK_DESCRIPTION_H__

#include <typeinfo>
#include <typeindex>

namespace chr {

namespace detail {

// forward declaration
class SlotBase;

} // namespace detail

class CallbackDescription {

public:

	CallbackDescription(
			std::type_index signalTypeIndex,
			std::type_index functionTypeIndex,
			void*           obj) :
		_signalTypeIndex(signalTypeIndex),
		_functionTypeIndex(functionTypeIndex),
		_obj(obj),
		_precedence(0) {}

	virtual ~CallbackDescription() {};

	/**
	 * Get the signal type index of the signal expected by the callback.
	 */
	std::type_index getSignalTypeIndex() { return _signalTypeIndex; }

	/**
	 * Called, whenever a compatible slot is trying to connect to this callback.  
	 * The connection will be established, if a non-zero delegate is returned.  
	 * To be overwritten by subclasses.
	 */
	virtual void* notifySlotConnect(detail::SlotBase* const) = 0;

	/**
	 * Called, whenever a compatible slot is trying to disconnect from the 
	 * delegate of this description (even if it was not successfully connected 
	 * before). To be overwritten by subclasses.
	 */
	virtual void notifySlotDisconnect(detail::SlotBase* const) {};

	/**
	 * Compare two callback descriptors: Returns true, if both descriptors point 
	 * to the same callback.
	 */
	bool operator==(const CallbackDescription& other) {

		return (_obj == other._obj && _functionTypeIndex == other._functionTypeIndex);
	}

	/**
	 * Set the precedence of the callback. Callbacks of a Receiver will be 
	 * checked in decreasing order of precedence for type compatibility with a 
	 * Slot. Only the first compatible callback will be connected. Callbacks 
	 * with the same precedence will be checked in the same order they have been 
	 * added to the Receiver. The default precedence is 0.
	 */
	void setPrecedence(int precedence) {

		_precedence = precedence;
	}

	int getPrecedence() const { return _precedence; }

private:

	// type of the signal accepted by the callback
	std::type_index _signalTypeIndex;

	// type of callback function for comparison
	std::type_index _functionTypeIndex;

	// object pointers for comparison
	void* _obj;

	// priority over other callbacks
	int _precedence;
};

} // namespace chr

#endif // COHEAR_CALLBACK_DESCRIPTION_H__

