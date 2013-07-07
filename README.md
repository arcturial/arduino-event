Arduino Event Manager
=====================

This library brings the principle of Events/Hooks to Arduino. In short, it allows a user
to respond to certain "events" quite easily.

### Table of Contents
* Installation
* Usage
* Timed Events
* Event Arguments


1. Installation
---------------

Download the Event.cpp & Event.h file and move it to your Arduino sketch repository. Once this is done, include the library in your sketch.

``` cpp

#include "Event.h"

```


2. Usage
--------

First off, we need to initialize a new EventManager. The EventManager will be responsible for registering new subscribers and triggering events when they occur.

``` cpp

EventManager evtManager;

```

Now that we have a new EventManager. Let's start listening for a new event.

``` cpp

EventManager evtManager;

void setup() 
{
	evtManager.subscribe(Subscriber("led.on", &LEDListener));
}

```

Notice that we use a reference to a listener as our "callback". So we would need to create this callback. The callback struct needs to extend the EventTask interface.

``` cpp

EventManager evtManager;
int ledPin = 12;

struct LEDListener : public EventTask
{
  using EventTask::execute;
  
  void execute(Event evt)
  {
  	digitalWrite(ledPin, HIGH);
  }

} LEDListener;

void setup() 
{
	pinMode(ledPin, OUTPUT);
	evtManager.subscribe(Subscriber("led.on", &LEDListener));
}

```

Now that we have a new listener registered for the event "led.on", we actually need to send a signal to trigger the event.

``` cpp

EventManager evtManager;
int ledPin = 12;

struct LEDListener : public EventTask
{
  using EventTask::execute;
  
  void execute(Event evt)
  {
  	digitalWrite(ledPin, HIGH);
  }

} LEDListener;

void setup() 
{
	pinMode(ledPin, OUTPUT);
	evtManager.subscribe(Subscriber("led.on", &LEDListener));
}

void loop()
{
	Event ledEvent = new Event("led.on");
	evtManager.trigger(ledEvent);
}

```

There we go, an event to light the LED at pin 12 triggers every "loop" interval. This might seem a bit redundant, but it's just an example. Event's won't be of much use for small applications with one purpose, they do however make large applications with many commands easier to maintain.


3. Timed Events
---------------

You can set up intervals to trigger certain events every X amount of milliseconds.

``` cpp

void setup()
{
	Event keepAlive = Event("event.keepAlive");
    evtManager.triggerInterval(TimedTask(2000, keepAlive));  
}

```

This piece of code will set up the EventManager to trigger a new "event.keepAlive" event every 2000 milliseconds (2 seconds).

4. Event Arguments
------------------

You can also use one event to do multiple things. Let's say we want one LED event to either turn it on or off.

``` cpp

...


struct LEDListener : public EventTask
{
  using EventTask::execute;
  
  void execute(Event evt)
  {
  	String extra = evt.extra;

  	if (extra == "on")
  	{
  		digitalWrite(ledPin, HIGH);
  	}
  	else
  	{
  		digitalWrite(ledPin, LOW);
  	}
  }

} LEDListener;

...

void loop()
{
	Event ledEvent = new Event("led.on", "on");
	evtManager.trigger(ledEvent);

	delay(1000);

	Event ledEvent = new Event("led.on", "off");
	evtManager.trigger(ledEvent);

	delay(1000);
}

```

This will turn raise a new "on" event and one second after that, raise an event to turn the LED off again. Thus, this will result in a "blinking LED" sketch.