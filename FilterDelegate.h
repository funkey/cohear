#ifndef COHEAR_FILTER_DELEGATE_H__
#define COHEAR_FILTER_DELEGATE_H__

#include "delegate/delegate.hpp"

namespace chr {

/**
 * A filter delegate is like an ordinary Delegate<SignalType>, but it returns a 
 * bool indicating whether the signal should be passed on.
 */
template <typename SignalType>
using FilterDelegate = detail::delegate<bool(SignalType&)>;

} // namespace chr

#endif // COHEAR_FILTER_DELEGATE_H__

