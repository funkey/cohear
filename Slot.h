#ifndef COHEAR_SLOT_H__
#define COHEAR_SLOT_H__

#include <set>
#include "detail/SlotBase.h"
#include "InheritanceMatchDelegateStrategy.h"

namespace chr {

template <
		typename SignalType,
		typename GetDelegateStrategy = InheritanceMatchDelegateStrategy<SignalType>>
class Slot : public detail::SlotBase {

public:

	Slot() :
		_delegatesNeedUpdate(false) {}

	void connect(Receiver& receiver) override final {

		// connect to each callback group of the receiver
		for (const Receiver::groups_type::value_type& groupCds : receiver.getCallbackDescriptions())
			connect(groupCds.second);
	}

	void disconnect(Receiver& receiver) override final {

		// disconnect from each callback group of the receiver
		for (const Receiver::groups_type::value_type& groupCds : receiver.getCallbackDescriptions())
			disconnect(groupCds.second);
	}

	virtual bool isCompatible(CallbackDescription* cd) override {

		return GetDelegateStrategy::IsCompatible(*cd);
	}

	template <typename ... Args>
	inline void operator()(Args ... args) {

		SignalType signal(args...);

		operator()(signal);
	}

	inline void operator()(SignalType& signal) {

		if (_delegatesNeedUpdate) {

			addStagedDelegates();
			removeStaleDelegates();

			_delegatesNeedUpdate = false;
		}

		for (auto& dd : _delegates)
			dd.delegate(signal);
	}

private:

	struct DescribedDelegate {

		Delegate<SignalType> delegate;
		CallbackDescription* description;

		bool operator==(const DescribedDelegate& other) const {

			return *description == *other.description;
		}

		bool operator<(const DescribedDelegate& other) const {

			if (*this == other)
				return false;
			else
				return description < other.description;
		}
	};

	typedef std::vector<DescribedDelegate> dds_list_type;
	typedef std::set<DescribedDelegate>    dds_set_type;

	void connect(const std::vector<CallbackDescription*>& cds) {

		for (auto* cd : cds) {

			if (connected(cd))
				break;

			if (isCompatible(cd)) {

				// ask the connection description about a delegate we can 
				// connect to
				void* untypedDelegate = cd->notifySlotConnect(this);

				// the callback offered us a delegate to connect to
				if (untypedDelegate) {

					Delegate<SignalType> delegate = GetDelegateStrategy::CastDelegate(untypedDelegate);
					_stagedDelegates.insert({delegate,cd});
					_delegatesNeedUpdate = true;

					// connect only to the first matching callback in each group
					break;
				}
			}
		}
	}

	void disconnect(const std::vector<CallbackDescription*>& cds) {

		for (auto* cd : cds) {

			// notify the connection description about our disconnect attempt, 
			// even if we haven't been connected
			cd->notifySlotDisconnect(this);

			for (typename dds_list_type::iterator i = _delegates.begin(); i != _delegates.end(); ++i) {

				if (*i->description == *cd) {

					_staleDelegates.insert(*i);
					_delegatesNeedUpdate = true;
					break;
				}
			}

			for (typename dds_set_type::iterator i = _stagedDelegates.begin(); i != _stagedDelegates.end(); ++i) {

				if (*i->description == *cd) {

					_stagedDelegates.erase(i);
					break;
				}
			}
		}
	}

	bool connected(CallbackDescription* cd) {

		// already connected, but marked for removal?
		for (auto& dd : _staleDelegates)
			if (*dd.description == *cd)
				return false;

		// already connected?
		for (auto& dd : _delegates)
			if (*dd.description == *cd)
				return true;

		// already connected, not yet added?
		for (auto& dd : _stagedDelegates)
			if (*dd.description == *cd)
				return true;

		return false;
	}

	void addStagedDelegates() {

		std::copy(_stagedDelegates.begin(), _stagedDelegates.end(), std::back_inserter(_delegates));
		_stagedDelegates.clear();
	}

	void removeStaleDelegates() {

		for (typename dds_set_type::iterator i = _staleDelegates.begin(); i != _staleDelegates.end(); ++i)
			for (typename dds_list_type::iterator j = _delegates.begin(); j != _delegates.end(); ++j)
				if (*i == *j) {

					_delegates.erase(j);
					break;
				}
		_staleDelegates.clear();
	}

	// currently connected delegates
	dds_list_type _delegates;

	// newly connected delegates, not yet added to _delegates
	dds_set_type  _stagedDelegates;

	// disconnected delegates, not yet removed from _delegates
	dds_set_type  _staleDelegates;

	// indicate that there are stale or staged delegates
	bool _delegatesNeedUpdate;
};

} // namespace chr

#endif // COHEAR_SLOT_H__

