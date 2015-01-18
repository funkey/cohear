/*
	(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

	Fast delegate compatible with C++ Standard.
*/
#ifndef COHEAR_DELEGATE_INCLUDED
#define COHEAR_DELEGATE_INCLUDED

// we always want the preferred syntax
#define COHEAR_DELEGATE_PREFERRED_SYNTAX

namespace chr {
namespace detail {
#ifdef COHEAR_DELEGATE_PREFERRED_SYNTAX
	template <typename TSignature> class delegate;
	template <typename TSignature> class delegate_invoker;
#endif
} // namespace detail
} // namespace chr

#ifdef _MSC_VER
#define COHEAR_DELEGATE_CALLTYPE __fastcall
#else
#define COHEAR_DELEGATE_CALLTYPE
#endif

#include "detail/delegate_list.hpp"

#undef COHEAR_DELEGATE_CALLTYPE

#endif//COHEAR_DELEGATE_INCLUDED
