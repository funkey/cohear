#ifndef COHEAR_META_GET_PARENT_H__
#define COHEAR_META_GET_PARENT_H__

namespace chr {

// symbol to indicate no parent
struct NoParent {};

namespace meta {

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

} // namespace meta

/**
 * Meta-function to get the parent type of a SignalType. If 
 * SignalType::parent_type is defined, this will be used. Other wise, the sybmol 
 * chr::NoParent will be returned.
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

	typedef typename meta::get_parent<SignalType, meta::has_parent_type<SignalType>::value>::value value;
};

} // namespace chr

#endif // COHEAR_META_GET_PARENT_H__

