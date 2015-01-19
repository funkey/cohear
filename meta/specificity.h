#ifndef COHEAR_DETAIL_SPECIFICITY_H__
#define COHEAR_DETAIL_SPECIFICITY_H__

#include "has_parent_type.h"
#include "get_parent.h"

namespace chr {
namespace meta {

template<typename T, typename P>
struct count_ancestors {

	static const int value = count_ancestors<P, typename parent_of<P>::value>::value + 1;
};

template<typename T>
struct count_ancestors<T, NoParent> {

	static const int value = 0;
};

template <typename T>
struct specificity {

	static const int value = count_ancestors<T, typename parent_of<T>::value>::value;
};

} // namespace meta
} // namespace chr

#endif // COHEAR_DETAIL_SPECIFICITY_H__

