#ifndef SR_SENDER_H__
#define SR_SENDER_H__

#include <vector>
#include "detail/SlotBase.h"
#include "Receiver.h"

namespace sr {

class Sender {

public:

	/**
	 * Register a slot with this sender. The sender will keep a pointer to the 
	 * slot, therefore the caller has to ensure that the slot remains valid 
	 * throughout the lifetime of the sender.
	 */
	void registerSlot(detail::SlotBase& slot) {

		_slots.push_back(&slot);
	}

	/**
	 * Connect the slots of this sender with the callbacks of the given 
	 * receiver.
	 */
	void connect(Receiver& receiver) {

		for (auto* slot : _slots)
			slot->connect(receiver);
	}

	/**
	 * Remove all connected callbacks of the given receiver from the slots of 
	 * this sender.
	 */
	void disconnect(Receiver& receiver) {

		for (auto* slot : _slots)
			slot->disconnect(receiver);
	}

private:

	std::vector<detail::SlotBase*> _slots;
};

} // namespace sr

#endif // SR_SENDER_H__

