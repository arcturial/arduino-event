/*
  Event
  
  This class serves as a "callback" manager to register events
  to happen on certain triggers or after certain intervals.
 */ 
 
#ifndef Event_h
#define Event_h

#include "Arduino.h"

/**
 * Event structure is the basic Event
 * object that can be dispatched by the
 * manager.
 */
struct Event
{
  Event() : label(NULL) {}
  Event(const char *cLabel) : label(cLabel) {}
  Event(const char *cLabel, const char *cExtra) : label(cLabel), extra(cExtra) {}
  const char *extra;
  const char *label;
};

/**
 * EventTask is a structure that serves as an
 * abstract class of a "dispatchable" object.
 */
struct EventTask
{
  virtual void execute(Event evt) = 0;
};

/**
 * The Subscriber is the object that
 * encapsulates the Event it's listening for
 * and the EventTask to be executed.
 */
struct Subscriber
{
  Subscriber() : label(NULL), task(NULL) {}
  Subscriber(const char *cLabel, EventTask *cTask) : label(cLabel), task(cTask) {}
  
  const char *label;
  EventTask *task;
};

/**
 * TimedTask is an Event that executes after a certain
 * amount of milliseconds.
 */
struct TimedTask
{
  TimedTask() : ms(0), evt(NULL) , alive(false), current(0)  {}
  TimedTask(unsigned long cMs, Event cEvt) : ms(cMs), evt(cEvt) , alive(true), current(60)  {}
  
  /**
   * Evaluates the state of the timed task and if
   * it's time to execute it or not. Resets the current
   * counter if it reaches the timed threshold.
   */
  boolean eval()
  {
    if (current >= ms)
    {
      current = 0;
      return true;
    }
    
    return false;
  }
  
  unsigned long ms;
  unsigned long current;
  Event evt;
  boolean alive; // State of the timed task
};

/**
 * The EventManager is responsible for gathering subscribers
 * and dispatching them when the requested Event is
 * triggered.
 */
class EventManager
{
  public:
    EventManager();
    void subscribe(Subscriber sub);
    void trigger(Event evt);
    void triggerInterval(TimedTask timed);
    void tick();
  private:
    TimedTask _interval[5]; // 5 available interval slots
    unsigned int _intervalSize;
    unsigned int _intervalPos;
    Subscriber _sub[10]; // 10 available subscriber slots
    unsigned int _subSize;
    unsigned int _subPos;
    unsigned long _previousMs;
};

#endif