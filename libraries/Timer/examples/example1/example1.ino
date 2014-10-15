#include <softShield.h>
#include <Event.h>
#include <Timer.h>

Timer t;

int pin = 13;

void setup(){
  pinMode(pin, OUTPUT);
  t.pulse(pin, 10 * 60 * 1000, HIGH); // 10 minutes  
}

void loop(){
  softModule::update();
}

