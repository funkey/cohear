#ifndef COHEAR_INHERITANCE_MATCH_DELEGATE_STRATEGY_H__
#define COHEAR_INHERITANCE_MATCH_DELEGATE_STRATEGY_H__

#include "Delegate.h"
#include "CallbackDescription.h"
#include "detail/SignalTraits.h"

namespace chr {

/**
 * A get-delegate-strategy, that creates a delegate from a callback description 
 * for a slot, if the signal type of the callback is equal or a base of the 
 * signal type of the slot.
 */
template <typename SignalType, typename AsSignalType = SignalType>
class InheritanceMatchDelegateStrategy {

public:

	static bool IsCompatible(CallbackDescription& cd) {

		if (ExactMatchDelegateStrategy<AsSignalType>::IsCompatible(cd))
			return true;

		typedef typename SignalTraits<AsSignalType>::parent_type ParentType;
		return InheritanceMatchDelegateStrategy<SignalType, ParentType>::IsCompatible(cd);
	}

	static Delegate<SignalType> CastDelegate(void* delegate) {

		// The following line assumes that the memory layout of Delegate<ToType> 
		// and Delegate<FromType> is the same (which it is, unless someone 
		// specializes Delegate<T> [their own fault]), and that 
		// Delegate<ToType>::operator() is doing what we would expect.  The 
		// latter is probably guarnateed by the C0x-standard (see below), but at 
		// least true in practice for a lot of compilers/architectures. For 
		// instance, GLib is doing much more evil casts without problems (see 
		// http://stackoverflow.com/questions/559581/casting-a-function-pointer-to-another-type).
		//
		// What happens: We tread Delegate<Base> as a Delegate<Derived>, 
		// where Derived is derived from Base. The differences between 
		// Delegate<Base> and Delegate<Derived> are just the type of the 
		// stub function pointer: They are (void(*)(Base&)) and 
		// (void(*)(Derived&)), respectively. So what we do, is basically a 
		// cast of a function pointer. The C0x-standard says to that:
		//
		//   [6.3.2.3]
		//   (http://c0x.coding-guidelines.com/6.3.2.3.html)
		//
		//   768 If a converted pointer is used to call a function whose 
		//   type is not compatible with the pointed-to type, the behavior 
		//   is undefined.
		//
		// Whether the following is guaranteed to work by the standard boils 
		// down to the question whether the function pointers are 
		// compatible. In [6.7.5.3], the standard says (amongst a lot of 
		// other things):
		//
		//   [6.3.2.3]
		//   (http://c0x.coding-guidelines.com/6.7.5.3.html)
		//
		//   1611 For two function types to be compatible, both shall 
		//   specify compatible return types.
		//
		// True.
		//
		//   1612 Moreover, the parameter type lists, if both are present, 
		//   shall agree in the number of parameters and in use of the 
		//   ellipsis terminator;
		//
		// True.
		//
		//   1613 corresponding parameters shall have compatible types.
		//
		// The parameters are references. If the same rules as for pointers 
		// apply (not sure), we have:
		//
		//   [6.7.5.1]
		//   (http://c0x.coding-guidelines.com/6.7.5.1.html)
		//
		//   1562 For two pointer types to be compatible, both shall be 
		//   identically qualified and both shall be pointers to compatible 
		//   types.
		//
		// Derived and Base are compatible. All that seems to tell us that 
		// we are fine.

		return *reinterpret_cast<Delegate<SignalType>*>(delegate);
	}
};

// template specialization for the no-parent case
template <typename SignalType>
class InheritanceMatchDelegateStrategy<SignalType, NoParent> {

public:

	static bool IsCompatible(CallbackDescription&) {

		return false;
	}

	static Delegate<SignalType> CastDelegate(void* delegate) {

		return Delegate<SignalType>();
	}
};

} // namespace chr

#endif // COHEAR_INHERITANCE_MATCH_DELEGATE_STRATEGY_H__

