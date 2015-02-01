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

	/**
	 * Returns true, if SignalType is compatible to the callback's signal type.
	 */
	static bool IsCompatible(CallbackDescription& cd) {

		return (std::type_index(typeid(SignalType)) == cd.getSignalTypeIndex());
	}

	/**
	 * Cast a delegate of a compatible callback to our expected delegate type.
	 */
	static Delegate<SignalType> CastDelegate(void* delegate) {

		return *static_cast<Delegate<SignalType>*>(delegate);
	}
};

} // namespace chr

#endif // COHEAR_EXACT_MATCH_DELEGATE_STRATEGY_H__

