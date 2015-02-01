#ifndef COHEAR_DETAIL_SLOT_BASE_H__
#define COHEAR_DETAIL_SLOT_BASE_H__

namespace chr {

// forward declarations
class Receiver;
class CallbackDescription;

namespace detail {

class SlotBase {

public:

	virtual ~SlotBase() {}

	/**
	 * Connect this slot to the callbacks of the given receiver.
	 */
	virtual void connect(Receiver& receiver) = 0;

	/**
	 * Disonnect this slot from the connected callbacks of the given receiver.
	 */
	virtual void disconnect(Receiver& receiver) = 0;

	/**
	 * Check, if this slot's signal is compatible to the callback's signal.
	 */
	virtual bool isCompatible(CallbackDescription* cd) = 0;
};

} // namespace detail
} // namespace chr

#endif // COHEAR_DETAIL_SLOT_BASE_H__

