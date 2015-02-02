#ifndef COHEAR_FILTER_H__
#define COHEAR_FILTER_H__

#include "FilterDelegate.h"
#include "detail/FilterCallbackDescription.h"
#include "detail/FilterSlot.h"

namespace chr {

/**
 * A callback-slot pair, that forwards and modifies any signal that is 
 * compatible with SignalType. The filter and unfilter functions are delegates 
 * provided by the user. Usage:
 *
 *   // the receiver to listen for signals
 *   Receiver receiver;
 *   // the sender to forward the filtered signal to
 *   Sender sender;
 *
 *   // filterDelegate and unfilterDelegate are delegates to functions with the
 *   // signature bool(A& a)
 *   Filter<A> filter(filterDelegate, unfilterDelegate);
 *
 *   receiver.registerCallback(filter.getCallback());
 *   sender.registerSlot(filter.getSlot());
 */
template <typename SignalType>
class Filter {

public:

	Filter(
			FilterDelegate<SignalType> filter,
			FilterDelegate<SignalType> unfilter) :
		_slot(filter, unfilter) {}

	/**
	 * Create and return a new filter callback description that forwards to the 
	 * slot of this filter.
	 */
	CallbackDescription* getCallback() {

		return new detail::FilterCallbackDescription<SignalType>(&_slot);
	}

	/**
	 * Get a reference to the output slot of this filter.
	 */
	detail::FilterSlot<SignalType>& getSlot() {

		return _slot;
	}

private:

	detail::FilterSlot<SignalType> _slot;
};

} // namespace chr

#endif // COHEAR_FILTER_H__

