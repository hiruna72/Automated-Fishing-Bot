#include<SPI.h>
#include<RF24.h>
#include <string.h>


#define BotID 0001  

// Modes
enum {RANDOM_MOVE,INIT_FISHING,DETECT_FISH,START_SLIDER,REVERSE_SLIDER,RELAY_CONTACT,ACK_LISTEN};

#define SENSING_THRESHOLD  0.7  // to normalize for different environments
#define DECISIVE_THRESHOLD 0.7  // decide whether  fish is inside
#define SENSE_TIME_GAP 5000     // this much of miliseconds between each sensing
#define SENSING_ITERATIONS 1  // how many sensing attempts at a one go




#define IRsensor A0
#define CE 9
#define CSN 10

//motor power switch
#define POWER_MOTOR 50
// motor one
#define ENA 7
#define IN1 6
#define IN2 5
// motor two
#define ENB 2
#define IN3 4
#define IN4 3

#define RANDOM_MOVE_TIME 10000
#define GET_STABLE_TIME 5000
//keeps a count how many times the IR sensor went HIGH 
int fishCount =0;

//keeps prev value of IRsensor reading
int IRprev = 1000;

//functions
void checkIRSensor();
bool moveSlider(bool);
int sendState(int );
int receiveState();
void random_move();