<!--- vim:spell:tw=80:fo=tawn
--->
cohear
======

**cohear** is a simple and fast header-only signalling library based on a 
sender-receiver metaphor: A `Sender` is a collection of `Slot`s that send 
signals (instances of a user-provided type). A `Receiver` is a collection of 
callbacks, each accepting a signal of a certain type.  When a `Sender` is 
connected to a `Receiver`, all compatible slot-callback pairs are automatically 
connected.  Compatibility is determined by inspection of the involved signal 
types: a `Slot` is compatible to a callback, if the `Slot`'s signal type is 
equal to or inherited from the callback's signal type.

**cohear** is build around [impossibly fast C++ delegates](
http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates).
Consequently, sending a signal is not more expensive than an ordinary function
call and even outperforms virtual function calls.

Usage Example
-------------

The following example assumes user provided signals `A` and `B` (with 
constructors `A()` and`B(int)`), and a handler implementation with the following 
interface:

    class Handler {

    public:

      void onA(A& signal);
      void onB(B& signal);
    };

### Creating a `Sender`

    chr::Slot<A> a;
    chr::Slot<B> b;

    chr::Sender sender;
    sender.registerSlot(a);
    sender.registerSlot(b);

### Creating a `Receiver`

    chr::Receiver receiver;
    receiver.registerCallback<A, Handler, &Handler::onA>(&handler);
    receiver.registerCallback<B, Handler, &Handler::onB>(&handler);

### Connecting `Sender` to `Receiver`

    sender.connect(receiver);

### Sending signals

    a();  // default constructed signal A
    b(5); // constructs B(5)
