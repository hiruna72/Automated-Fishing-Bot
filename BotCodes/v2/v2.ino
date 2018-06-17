//high levle multitasking library
#include <SoftTimer.h>  
//analog pin 0
//IR sensor 

//<<<<<<<<<<<<PINS
// value is positive if fish is present
int IRsensor = A0; 



//>>>>>>>>>>>>>>>>>>>>>>>>



//<<<<<<<<<<<<<<<VARIABLES

//keeps a count how many times the IR sensor went HIGH 
int fishCount =0;

//how many times the IR sensor is checked in a one go
int checksForOneGO = 10;

//threshold to make decision that fish is inside the trap
int countThreshold = 0.6;

//factor to mark a threshold for fish presence
float fishDetectFactor = 0.75;

//keeps prev value of IRsensor reading
int IRprev = 1000;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>




//<<<<<<<<<<<<<<states
boolean detectFish = true;

//>>>>>>>>>>>>>>>>>>>


// -- chechIR will be launched on every 2 seconds.
Task checkIR(2000, checkIRSensor);
Task printFishCount(10000, fishCountPrinter);

void setup(){
  //register checkIR task 
  SoftTimer.add(&checkIR);
  //register printFishCount task 
  SoftTimer.add(&printFishCount);
  Serial.begin(9600);
}

void checkIRSensor(Task* me) {
  if(!detectFish){
    return;
  }
  int IRreading = analogRead(IRsensor);
  if(fishDetectFactor * IRprev > IRreading){
      fishCount++;
    } 
    else{
      IRprev = IRreading;
    }
}

//to print fish count
void fishCountPrinter(Task* me) {
  if(fishCount > checksForOneGO*countThreshold){
    Serial.println("fish inside the trap!");
    Serial.println(fishCount);
  }
  fishCount =0 ;
}

