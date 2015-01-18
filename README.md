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

### Creating Signals

Signals are just user-provided classes with inheritance cues:

```c++
class A {};

class A1 : public A {
public:
  typedef A parent_type; // inheritance cue
};

class B() {
public:
  B(int n) : _n(n) {}
  /*...*/
private:
  int _n;
};
```

### Callback Interface

Any method with the signature `void(Signal&)` can be used as a callback for type 
`Signal`:

```c++
class Handler {

public:

  void onA(A& signal);
  void onB(B& signal);
};
```

### Creating a `Sender`

A sender is just a collection of signal `Slot`s:

```c++
chr::Slot<A>  a;
chr::Slot<A1> a1;
chr::Slot<B>  b;

chr::Sender sender;
sender.registerSlot(a);
sender.registerSlot(a1);
sender.registerSlot(b);
```

### Creating a `Receiver`

Analogously, a receiver is a collection of callbacks:

```c++
Handler handler;

chr::Receiver receiver;
receiver.registerCallback<A, Handler, &Handler::onA>(&handler);
receiver.registerCallback<B, Handler, &Handler::onB>(&handler);
```

### Connecting `Sender` to `Receiver`

    sender.connect(receiver);

### Sending Signals

Signals are sent by invoking the `operator()` on the respective `Slot`s:

```c++
a();  // calls handler.onA() with signal A()
a1(); // calls handler.onA() with signal A1()
b(5); // calls handler.onB() with signal B(5)
```
