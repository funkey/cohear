#ifndef SR_DETAIL_SLOT_BASE_H__
#define SR_DETAIL_SLOT_BASE_H__

namespace sr {

// forward declaration
class Receiver;

namespace detail {

class SlotBase {

public:

	/**
	 * Connect this slot to the callbacks of the given receiver.
	 */
	virtual void connect(Receiver& receiver) = 0;

	/**
	 * Disonnect this slot from the connected callbacks of the given receiver.
	 */
	virtual void disconnect(Receiver& receiver) = 0;
};

} // namespace detail
} // namespace sr

#endif // SR_DETAIL_SLOT_BASE_H__

