#ifndef COHEAR_EXACT_MATCH_DELEGATE_STRATEGY_H__
#define COHEAR_EXACT_MATCH_DELEGATE_STRATEGY_H__

#include "Delegate.h"
#include "CallbackDescription.h"

namespace chr {

/**
 * A get-delegate-strategy, that creates a delegate from a callback description 
 * for a slot, if the signal type of the callback and slot do match exactly.
 */
template <typename SignalType>
class ExactMatchDelegateStrategy {

public:

	static bool GetDelegate(CallbackDescription& cd, Delegate<SignalType>& delegate) {

		if (std::type_index(typeid(SignalType)) == cd.getSignalTypeIndex()) {

			// we only update the delegate, if the callback description did have 
			// a non-zero delegate pointer
			if (cd.getDelegate())
				delegate = *static_cast<Delegate<SignalType>*>(cd.getDelegate());

			return true;
		}

		// no match
		return false;
	}
};

} // namespace chr

#endif // COHEAR_EXACT_MATCH_DELEGATE_STRATEGY_H__

