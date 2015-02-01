#ifndef COHEAR_DETAIL_INTERNAL_FILTER_SLOT_H__
#define COHEAR_DETAIL_INTERNAL_FILTER_SLOT_H__

#include <cohear/Delegate.h>
#include <cohear/Slot.h>
#include <cohear/FilterDelegate.h>

namespace chr {
namespace detail {

template <typename SignalType>
class InternalFilterSlot : public Slot<SignalType> {

public:

	InternalFilterSlot(SlotBase* originalSlot) :
		_originalSlot(originalSlot),
		_outerDelegate(
				Delegate<SignalType>::template from_method<
					InternalFilterSlot<SignalType>,
					&InternalFilterSlot<SignalType>::filterAndForward>(this)) {}

	/**
	 * Set the filter and unfilter delegates.
	 */
	void setFilterDelegates(
			FilterDelegate<SignalType> filter,
			FilterDelegate<SignalType> unfilter) {

		_filter   = filter;
		_unfilter = unfilter;
	}

	void filterAndForward(SignalType& signal) {

		if (_filter(signal)) {

			this->operator()(signal);
			_unfilter(signal);
		}
	}

	/**
	 * Get a delegate to the filterAndForward method of this slot. This delegate 
	 * will be used in slots that connected to the corresponding 
	 * FilterCallbackDescription (and is therefore called the "outer" delegate).
	 */
	Delegate<SignalType>* getOuterDelegate() {

		return &_outerDelegate;
	}

	bool isCompatible(CallbackDescription* cd) override {

		// this slot is compatible, with the given callback, if the original 
		// call back is
		return _originalSlot->isCompatible(cd);
	}

private:

	FilterDelegate<SignalType> _filter;
	FilterDelegate<SignalType> _unfilter;

	// the original slot this internal filter slot was created for
	SlotBase* _originalSlot;

	Delegate<SignalType> _outerDelegate;
};

} // namespace detail
} // namespace chr

#endif // COHEAR_DETAIL_INTERNAL_FILTER_SLOT_H__

