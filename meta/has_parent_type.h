#ifndef COHEAR_META_HAS_PARENT_TYPE_H__
#define COHEAR_META_HAS_PARENT_TYPE_H__

namespace chr {
namespace meta {

////////////////////////////////////////////////////////////////////////////////
// has_parent_type template (stolen from stackoverflow)
////////////////////////////////////////////////////////////////////////////////

template<typename> struct tovoid { typedef void parent_type; };

template<typename T, typename = void> struct has_parent_type : std::false_type { };

// this one will only be selected if C::parent_type is valid
template<typename C> struct has_parent_type<C, typename tovoid<typename C::parent_type>::parent_type> : std::true_type { };

} // namespace meta
} // namespace chr

#endif // COHEAR_META_HAS_PARENT_TYPE_H__

