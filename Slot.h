#ifndef COHEAR_SLOT_H__
#define COHEAR_SLOT_H__

#include <map>
#include "detail/SlotBase.h"
#include "ExactMatchDelegateStrategy.h"

namespace chr {

template <
		typename SignalType,
		typename GetDelegateStrategy = ExactMatchDelegateStrategy<SignalType>>
class Slot : public detail::SlotBase {

public:

	void connect(Receiver& receiver) override final {

		for (auto* cd : receiver.getCallbackDescriptions()) {

			if (connected(cd))
				break;

			Delegate<SignalType> delegate;
			bool foundMatch = GetDelegateStrategy::GetDelegate(*cd, delegate);

			if (foundMatch) {

				std::cout << "match found" << std::endl;

				// notify the connection description about our connect attempt, 
				// even if we didn't get a delegate
				cd->notifySlotConnect(this);

				if (delegate) {

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

			for (int i = 0; i < _delegates.size(); i++) {

				if (*_delegates[i].description == *cd) {

					_delegates.erase(_delegates.begin() + i);
					break;
				}
			}
		}
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

