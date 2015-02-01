#ifndef COHEAR_FILTER_CALLBACK_DESCRIPTION_H__
#define COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

#include "CallbackDescription.h"
#include "FilterSlot.h"
#include "FilterDelegate.h"

namespace chr {

template <typename SignalType>
class FilterCallbackDescription : public CallbackDescription {

public:

	FilterCallbackDescription(
			FilterSlot<SignalType>* slot,
			FilterDelegate<SignalType> filter,
			FilterDelegate<SignalType> unfilter) :
		CallbackDescription(
				typeid(SignalType),
				typeid(*this),
				slot),
		_slot(slot),
		_filter(filter),
		_unfilter(unfilter) {}

	void* notifySlotConnect(detail::SlotBase* const slot) override final {

		return _slot->registerOriginalSlot(slot, _filter, _unfilter);
	}

	void notifySlotDisconnect(detail::SlotBase* const slot) override final {

		_slot->unregisterOriginalSlot(slot);
	}

private:

	// the slot to forward signals to
	FilterSlot<SignalType>* _slot;

	// the actual filter delegates
	FilterDelegate<SignalType> _filter;
	FilterDelegate<SignalType> _unfilter;
};

} // namespace chr

#endif // COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

