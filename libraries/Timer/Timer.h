#ifndef Timer_h
#define Timer_h

#if ARDUINO < 100
	#include <WProgram.h>
#else
	#include <Arduino.h>
#endif

#include <softShield.h>
#include "Event.h"

#define MAX_NUMBER_OF_EVENTS (10)

#define TIMER_NOT_AN_EVENT (-2)
#define NO_TIMER_AVAILABLE (-1)

class Timer : private softShield
{

public:
  Timer(void);

  int8_t every(unsigned long period, void (*callback)(void));
  int8_t every(unsigned long period, void (*callback)(void), int repeatCount);
  int8_t after(unsigned long duration, void (*callback)(void));
  int8_t oscillate(uint8_t pin, unsigned long period, uint8_t startingValue);
  int8_t oscillate(uint8_t pin, unsigned long period, uint8_t startingValue, int repeatCount);
  
  /**
   * This method will generate a pulse of !startingValue, occuring period after the
   * call of this method and lasting for period. The Pin will be left in !startingValue.
   */
  int8_t pulse(uint8_t pin, unsigned long period, uint8_t startingValue);
  
  /**
   * This method will generate a pulse of pulseValue, starting immediately and of
   * length period. The pin will be left in the !pulseValue state
   */
  int8_t pulseImmediate(uint8_t pin, unsigned long period, uint8_t pulseValue);
  void stop(int8_t id);
  virtual void update(void)
  {
	unsigned long now = millis();
	this->update(now);
  }
  virtual void setup(void){}
  void update(unsigned long now);

protected:
  Event _events[MAX_NUMBER_OF_EVENTS];
  int8_t findFreeEventIndex(void);

};

#endif
