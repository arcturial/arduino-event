/*
  Event
  
  This class serves as a "callback" manager to register events
  to happen on certain triggers or after certain intervals.
 */
 
#include "Arduino.h"
#include "Event.h"

/**
 * Constructs a new EventManager and
 * figures out the size of the available
 * array slots.
 */
EventManager::EventManager()
{
  _intervalSize = sizeof(_interval) / sizeof(TimedTask);
  _intervalPos = 0;
  
  _subSize = sizeof(_sub) / sizeof(Subscriber);
}

/**
 * Subscribes a new Subscriber to the
 * event manager.
 */
void EventManager::subscribe(Subscriber sub)
{
  if (_subSize >= _subPos)
  {
    _sub[_subPos] = sub;
    _subPos++;
  }
}

/** 
 * Triggers a specified event which will find the applicable
 * Subscriber and execute it's EventTask
 */
void EventManager::trigger(Event evt)
{
  for (int i = 0; i < _subSize; i++)
  {
    Subscriber *sub = &_sub[i];
    
    if ((String) sub->label == (String) evt.label)
    {
      // Execute event
      sub->task->execute(evt);
    }  
  }
}

/**
 * Setup a timed trigger that will execute an
 * event after a couple of milliseconds.
 */
void EventManager::triggerInterval(TimedTask task)
{
  if (_intervalSize >= _intervalPos)
  {
    _interval[_intervalPos] = task;
    _intervalPos++;
  }
}

/**
 * Tick the EventManager to evaluate any
 * timed instances for the manager.
 */
void EventManager::tick()
{
  unsigned long currentMs = millis();
  unsigned long difference = currentMs - _previousMs;
  
  for (int i = 0; i < _intervalSize; i++)
  {
    TimedTask *task = &_interval[i];
    
    if (task->alive)
    {
      task->current = task->current + difference;
      
      if (task->eval())
      {
        // Run the timed event when it evalutes to
        // ready.
        trigger(task->evt);
      }
    }
  }
  
  _previousMs = currentMs;
}