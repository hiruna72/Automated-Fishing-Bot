//high levle multitasking library
#include <SoftTimer.h>  
//analog pin 0
//IR sensor 
// value is positive if fish is present
int IRsensor = A0; 

//keeps a count how many times the IR sensor went HIGH 
int fishCount =0;

//how many times the IR sensor is checked in a one go
int checksForOneGO = 10;

//factor to mark a threshold for fish presence
float fishDetectFactor = 0.75;

//keeps prev value of IRsensor reading
int IRprev = 1000;

//whenever the IR sensor reads fish presence blink
int fishDetect = 8;


// -- chechIR will be launched on every 2 seconds.
Task checkIR(2000, checkIRSensor);
Task printFishCount(10000, fishCountPrinter);

void setup(){
  //register checkIR task 
  SoftTimer.add(&checkIR);
  //register printFishCount task 
  SoftTimer.add(&printFishCount);
  pinMode(fishDetect, OUTPUT);
  Serial.begin(9600);
  digitalWrite(fishDetect, LOW);
}

void checkIRSensor(Task* me) {
  int IRreading = analogRead(IRsensor);
  if(fishDetectFactor * IRprev > IRreading){
      digitalWrite(fishDetect, HIGH);
      fishCount++;
    } 
    else{
      IRprev = IRreading;
    }
}

//to print fish count
void fishCountPrinter(Task* me) {
  if(fishCount>ch
  Serial.println("inside");
  fishCount =0 ;
}
