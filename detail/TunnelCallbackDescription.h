#ifndef COHEAR_TUNNEL_CALLBACK_DESCRIPTION_H__
#define COHEAR_TUNNEL_CALLBACK_DESCRIPTION_H__

#include <cohear/CallbackDescription.h>
#include "TunnelSlot.h"

namespace chr {
namespace detail {

template <typename SignalType>
class TunnelCallbackDescription : public CallbackDescription {

public:

	TunnelCallbackDescription(TunnelSlot* slot) :
		CallbackDescription(
				typeid(SignalType),
				typeid(TunnelCallbackDescription),
				slot),
		_slot(slot) {
		// the more specific a signal, the higher the precedence of the callback
		setPrecedence(SignalTraits<SignalType>::specificity);
	}

	void* notifySlotConnect(detail::SlotBase* const slot) override final {

		_slot->addSlot(slot);
		return 0;
	}

	void notifySlotDisconnect(detail::SlotBase* const slot) override final {

		_slot->removeSlot(slot);
	}

private:

	// the slot to forward signals to
	TunnelSlot* _slot;
};

} // namespace detail
} // namespace chr

#endif // COHEAR_TUNNEL_CALLBACK_DESCRIPTION_H__

