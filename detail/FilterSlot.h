#ifndef COHEAR_FILTER_SLOT_H__
#define COHEAR_FILTER_SLOT_H__

#include "SlotBase.h"
#include "InternalFilterSlot.h"

namespace chr {
namespace detail {

template <typename SignalType>
class FilterSlot : public detail::SlotBase {

public:

	FilterSlot(
			FilterDelegate<SignalType> filter,
			Delegate<SignalType> unfilter) :
		_filter(filter),
		_unfilter(unfilter) {}

	~FilterSlot() {

		for (auto* slot : _internalSlots)
			delete slot;
	}

	void connect(Receiver& receiver) override {

		// remember we are connected to this receiver
		_receivers.push_back(&receiver);

		// connect all internal slots to the new receiver
		for (SlotBase* slot : _internalSlots)
			slot->connect(receiver);
	}

	void disconnect(Receiver& receiver) override {

		std::vector<Receiver*>::iterator i =
				std::find(_receivers.begin(), _receivers.end(), &receiver);
		if (i == _receivers.end())
			return;

		// forget about this receiver
		_receivers.erase(i);

		// disconnect all registered slots at the other side to the new receiver
		for (SlotBase* slot : _internalSlots)
			slot->disconnect(receiver);
	}

	bool isCompatible(CallbackDescription*) override {

		// A FilterSlot itself is always compatible (but the original slots 
		// might not be)
		return true;
	}

	/**
	 * To be called by FilterCallbackDescription, whenever a compatible slot was 
	 * connected. Returns a delegate to the internal slot's filterAndForward 
	 * method.
	 */
	Delegate<SignalType>* registerOriginalSlot(SlotBase* slot) {

		// create a new internal slot for the original slot
		detail::InternalFilterSlot<SignalType>* internalSlot = new detail::InternalFilterSlot<SignalType>(slot);
		internalSlot->setFilterDelegates(_filter, _unfilter);

		// remember the slot
		_internalSlots.push_back(internalSlot);

		// try to connect it to all currently connected receivers
		for (Receiver* receiver : _receivers)
			internalSlot->connect(*receiver);

		return internalSlot->getOuterDelegate();
	}

	/**
	 * To be called by FilterCallbackDescription, whenever a matching slot 
	 * was connected.
	 */
	void unregisterOriginalSlot(SlotBase* slot) {

		std::vector<SlotBase*>::iterator i = std::find(_internalSlots.begin(), _internalSlots.end(), slot);
		if (i == _internalSlots.end())
			return;

		delete *i;

		// forget about this slot
		_internalSlots.erase(i);

		// disconnect it from all currently connected receivers
		for (Receiver* receiver : _receivers)
			slot->disconnect(*receiver);
	}

private:

	// internal filter slots that filter the signal and fire themselves
	std::vector<SlotBase*> _internalSlots;

	// receivers connected to this slot
	std::vector<Receiver*> _receivers;

	// the actual filter delegates
	FilterDelegate<SignalType> _filter;
	Delegate<SignalType> _unfilter;
};

} // namespace detail
} // namespace chr

#endif // COHEAR_FILTER_SLOT_H__

