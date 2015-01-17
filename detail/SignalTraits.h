#ifndef SR_DETAILS_SIGNAL_TRAITS_H__
#define SR_DETAILS_SIGNAL_TRAITS_H__

#include <sr/meta/has_parent_type.h>

namespace chr {
namespace detail {

// symbol to indicate no parent
struct NoParent {};

// get_parent meta-function: get SignalType::parent_type if defined, NoParent
// otherwise
template <typename SignalType, bool HasParentTypeDef>
struct get_parent {};

template <typename SignalType>
struct get_parent<SignalType, true> {

	typedef typename SignalType::parent_type value;
};

template <typename SignalType>
struct get_parent<SignalType, false> {

	typedef NoParent value;
};

} // namespace detail

/**
 * Meta-function to get the parent type of a SignalType (used by SignalTraits).  
 * If SignalType::parent_type is defined, this will be used. Other wise, the 
 * sybmol NoParent will be returned.
 *
 * You can specialize this template for a non-invasive parent type definition:
 *
 *   template <>
 *   struct parent_of<MySignal> {
 *     typedef BaseSignal parent_type;
 *   };
 */
template <typename SignalType>
struct parent_of {

	typedef typename detail::get_parent<SignalType, meta::has_parent_type<SignalType>::value>::value value;
};

/**
 * Meta-function to query properties of signal types.
 */
template <typename SignalType>
struct SignalTraits {

	typedef typename parent_of<SignalType>::value parent_type;
};

} // namespace chr

#endif // SR_DETAILS_SIGNAL_TRAITS_H__

