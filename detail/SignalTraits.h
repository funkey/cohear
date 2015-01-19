#ifndef COHEAR_DETAILS_SIGNAL_TRAITS_H__
#define COHEAR_DETAILS_SIGNAL_TRAITS_H__

#include <cohear/meta/has_parent_type.h>
#include <cohear/meta/get_parent.h>
#include <cohear/meta/specificity.h>

namespace chr {

/**
 * Meta-function to query properties of signal types.
 */
template <typename SignalType>
struct SignalTraits {

	/**
	 * The parent of SignalType, or the symbol chr::NoParent if no parent was 
	 * specified.
	 */
	typedef typename parent_of<SignalType>::value parent_type;

	/**
	 * The number of ancestors of SignalType (according to the defined parents).
	 */
	static const int specificity = meta::specificity<SignalType>::value;
};

} // namespace chr

#endif // COHEAR_DETAILS_SIGNAL_TRAITS_H__

