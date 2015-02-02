#ifndef COHEAR_TUNNEL_H__
#define COHEAR_TUNNEL_H__

#include "detail/TunnelCallbackDescription.h"
#include "detail/TunnelSlot.h"

namespace chr {

/**
 * A callback-slot pair, that simply forward any signal that is compatible with 
 * SignalType from a receiver to a sender. Usage:
 *
 *   // the receiver to listen for signals
 *   Receiver receiver;
 *   // the sender to forward the signal to
 *   Sender sender;
 *
 *   Tunnel<A> tunnel;
 *
 *   receiver.registerCallback(tunnel.getCallback());
 *   sender.registerSlot(tunnel.getSlot());
 *
 * Tunnels come with no additional costs for sending signals, they just connect 
 * all compatible slots that are connected to the receiver with all compatible 
 * callbacks that are connected to the sender.
 */
template <typename SignalType>
class Tunnel {

public:

	/**
	 * Create and return a new tunnel callback description that forwards to the 
	 * slot of this tunnel.
	 */
	CallbackDescription* getCallback() {

		return new detail::TunnelCallbackDescription<SignalType>(&_slot);
	}

	/**
	 * Get a reference to the output slot of this tunnel.
	 */
	detail::TunnelSlot& getSlot() {

		return _slot;
	}

private:

	detail::TunnelSlot _slot;
};

} // namespace chr

#endif // COHEAR_TUNNEL_H__

