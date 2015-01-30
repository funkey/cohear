#ifndef COHEAR_FILTER_CALLBACK_DESCRIPTION_H__
#define COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

#include "CallbackDescription.h"
#include "FilterSlot.h"

namespace chr {

template <typename SignalType>
class FilterCallbackDescription : public CallbackDescription {

public:

	FilterCallbackDescription(FilterSlot* slot) :
		CallbackDescription(
				typeid(SignalType),
				typeid(*this),
				slot),
		_slot(slot) {}

	void* getDelegate() override { return 0; }

	void notifySlotConnect(detail::SlotBase* const slot) {

		_slot->addSlot(slot);
	}

	void notifySlotDisconnect(detail::SlotBase* const slot) {

		_slot->removeSlot(slot);
	}

private:

	// the slot to forward signals to
	FilterSlot* _slot;
};

} // namespace chr

#endif // COHEAR_FILTER_CALLBACK_DESCRIPTION_H__

