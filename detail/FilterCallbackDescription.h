#ifndef COHEAR_FILTER_CALLBACK_DESCRIPTION_H__
#define COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

#include <cohear/CallbackDescription.h>
#include <cohear/FilterDelegate.h>
#include "FilterSlot.h"

namespace chr {
namespace detail {

template <typename SignalType>
class FilterCallbackDescription : public CallbackDescription {

public:

	FilterCallbackDescription(FilterSlot<SignalType>* slot) :
		CallbackDescription(
				typeid(SignalType),
				typeid(FilterCallbackDescription),
				slot),
		_slot(slot) {

		// the more specific a signal, the higher the precedence of the callback
		setPrecedence(SignalTraits<SignalType>::specificity);
	}

	void* notifySlotConnect(detail::SlotBase* const slot) override final {

		return _slot->registerOriginalSlot(slot);
	}

	void notifySlotDisconnect(detail::SlotBase* const slot) override final {

		_slot->unregisterOriginalSlot(slot);
	}

private:

	// the slot to forward signals to
	FilterSlot<SignalType>* _slot;
};

} // namespace detail
} // namespace chr

#endif // COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

