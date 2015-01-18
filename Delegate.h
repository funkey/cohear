#ifndef COHEAR_DELEGATE_H__
#define COHEAR_DELEGATE_H__

#include "delegate/delegate.hpp"

namespace chr {

/**
 * Our delegate is a simple function taking only a reference to the signal that 
 * is sent.
 */
template <typename SignalType>
using Delegate = detail::delegate<void(SignalType&)>;

} // namespace chr

#endif // COHEAR_DELEGATE_H__

