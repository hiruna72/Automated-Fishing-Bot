#include <SoftTimer.h>

// -- taskOn will be launched on every 2 seconds.

// -- taskOff will be launched on every 1111 milliseconds.
Task taskOff(1111, turnOff);

void setup() {
  // -- Mark pin 13 as an output pin.
  pinMode(13, OUTPUT);

  // -- Register the tasks to the timer manager. Both tasks will start immediately.
  
  SoftTimer.add(&taskOff);
}

/**
 * Turn ON Arduino's LED on pin 13.
 */


/**
 * Turn OFF Arduino's LED on pin 13.
 */
void turnOff(Task* me) {
  digitalWrite(13, LOW);
}
