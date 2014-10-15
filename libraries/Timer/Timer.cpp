#include "Timer.h"

Timer::Timer(void)
{
}

int8_t Timer::every(unsigned long period, void (*callback)(), int repeatCount)
{
	int8_t i = findFreeEventIndex();
	if (i == -1) return -1;

	_events[i].eventType = EVENT_EVERY;
	_events[i].period = period;
	_events[i].repeatCount = repeatCount;
	_events[i].callback = callback;
	_events[i].lastEventTime = millis();
	_events[i].count = 0;
	return i;
}

int8_t Timer::every(unsigned long period, void (*callback)())
{
	return every(period, callback, -1); // - means forever
}

int8_t Timer::after(unsigned long period, void (*callback)())
{
	return every(period, callback, 1);
}

int8_t Timer::oscillate(uint8_t pin, unsigned long period, uint8_t startingValue, int repeatCount)
{
	int8_t i = findFreeEventIndex();
	if (i == NO_TIMER_AVAILABLE) return NO_TIMER_AVAILABLE;

	_events[i].eventType = EVENT_OSCILLATE;
	_events[i].pin = pin;
	_events[i].period = period;
	_events[i].pinState = startingValue;
	digitalWrite(pin, startingValue);
	_events[i].repeatCount = repeatCount * 2; // full cycles not transitions
	_events[i].lastEventTime = millis();
	_events[i].count = 0;
	return i;
}

int8_t Timer::oscillate(uint8_t pin, unsigned long period, uint8_t startingValue)
{
	return oscillate(pin, period, startingValue, -1); // forever
}

/**
 * This method will generate a pulse of !startingValue, occuring period after the
 * call of this method and lasting for period. The Pin will be left in !startingValue.
 */
int8_t Timer::pulse(uint8_t pin, unsigned long period, uint8_t startingValue)
{
	return oscillate(pin, period, startingValue, 1); // once
}

/**
 * This method will generate a pulse of startingValue, starting immediately and of
 * length period. The pin will be left in the !startingValue state
 */
int8_t Timer::pulseImmediate(uint8_t pin, unsigned long period, uint8_t pulseValue)
{
	int8_t id(oscillate(pin, period, pulseValue, 1));
	// now fix the repeat count
	if (id >= 0 && id < MAX_NUMBER_OF_EVENTS) {
		_events[id].repeatCount = 1;
	}
	return id;
}


void Timer::stop(int8_t id)
{
	if (id >= 0 && id < MAX_NUMBER_OF_EVENTS) {
		_events[id].eventType = EVENT_NONE;
	}
}



void Timer::update(unsigned long now)
{
	for (int8_t i = 0; i < MAX_NUMBER_OF_EVENTS; i++)
	{
		if (_events[i].eventType != EVENT_NONE)
		{
			_events[i].update(now);
		}
	}
}

int8_t Timer::findFreeEventIndex(void)
{
	for (int8_t i = 0; i < MAX_NUMBER_OF_EVENTS; i++)
	{
		if (_events[i].eventType == EVENT_NONE)
		{
			return i;
		}
	}
	return NO_TIMER_AVAILABLE;
}
