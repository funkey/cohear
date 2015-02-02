#ifndef COHEAR_SLOT_H__
#define COHEAR_SLOT_H__

#include <map>
#include "detail/SlotBase.h"
#include "InheritanceMatchDelegateStrategy.h"

namespace chr {

template <
		typename SignalType,
		typename GetDelegateStrategy = InheritanceMatchDelegateStrategy<SignalType>>
class Slot : public detail::SlotBase {

public:

	void connect(Receiver& receiver) override final {

		for (auto* cd : receiver.getCallbackDescriptions()) {

			if (connected(cd))
				break;

			if (isCompatible(cd)) {

				// ask the connection description about a delegate we can 
				// connect to
				void* untypedDelegate = cd->notifySlotConnect(this);

				// the callback offered us a delegate to connect to
				if (untypedDelegate) {

					Delegate<SignalType> delegate = GetDelegateStrategy::CastDelegate(untypedDelegate);
					_delegates.push_back({delegate,cd});

					// connect only to the first matching callback, this can be 
					// generalized by a strategy
					break;
				}
			}
		}
	}

	void disconnect(Receiver& receiver) override final {

		for (auto* cd : receiver.getCallbackDescriptions()) {

			// notify the connection description about our disconnect attempt, 
			// even if we haven't been connected
			cd->notifySlotDisconnect(this);

			for (std::size_t i = 0; i < _delegates.size(); i++) {

				if (*_delegates[i].description == *cd) {

					_delegates.erase(_delegates.begin() + i);
					break;
				}
			}
		}
	}

	virtual bool isCompatible(CallbackDescription* cd) override {

		return GetDelegateStrategy::IsCompatible(*cd);
	}

	template <typename ... Args>
	inline void operator()(Args ... args) {

		SignalType signal(args...);

		for (auto& dd : _delegates)
			dd.delegate(signal);
	}

	inline void operator()(SignalType& signal) {

		for (auto& dd : _delegates)
			dd.delegate(signal);
	}

private:

	struct DescribedDelegate {

		Delegate<SignalType> delegate;
		CallbackDescription* description;
	};

	bool connected(CallbackDescription* cd) {

		for (auto& dd : _delegates)
			if (*dd.description == *cd)
				return true;

		return false;
	}

	std::vector<DescribedDelegate> _delegates;
};

} // namespace chr

#endif // COHEAR_SLOT_H__

