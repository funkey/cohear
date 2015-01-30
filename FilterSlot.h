#ifndef COHEAR_FILTER_SLOT_H__
#define COHEAR_FILTER_SLOT_H__

#include "detail/SlotBase.h"

namespace chr {

class FilterSlot : public detail::SlotBase {

public:

	void connect(Receiver& receiver) override {

		// remember we are connected to this receiver
		_receivers.push_back(&receiver);

		// connect all registered slots at the other side to the new receiver
		for (SlotBase* slot : _slots)
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
		for (SlotBase* slot : _slots)
			slot->disconnect(receiver);
	}

	/**
	 * To be called by FilterCallbackDescription, whenever a matching slot 
	 * was connected.
	 */
	void addSlot(SlotBase* slot) {

		// remember the slot
		_slots.push_back(slot);

		// try to connect it to all currently connected receivers
		for (Receiver* receiver : _receivers)
			slot->connect(*receiver);
	}

	/**
	 * To be called by FilterCallbackDescription, whenever a matching slot 
	 * was connected.
	 */
	void removeSlot(SlotBase* slot) {

		std::vector<SlotBase*>::iterator i = std::find(_slots.begin(), _slots.end(), slot);
		if (i == _slots.end())
			return;

		// forget about this slot
		_slots.erase(i);

		// disconnect it from all currently connected receivers
		for (Receiver* receiver : _receivers)
			slot->disconnect(*receiver);
	}

private:

	// slots that are compatible with the FilterCallbackDescription on the 
	// other side
	std::vector<SlotBase*> _slots;

	// receivers connected to this slot
	std::vector<Receiver*> _receivers;
};

} // namespace chr

#endif // COHEAR_FILTER_SLOT_H__

