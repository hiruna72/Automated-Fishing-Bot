#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>

#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Arduino.h>
#include "BasicStepperDriver.h"

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

#include <SPI.h>
#include <RF24.h>
#include <string.h>

// Modes
enum {RANDOM_MOVE,INIT_FISHING,DETECT_FISH,START_SLIDER,RELAY_CONTACT, DAMAGE_CONTROL};


#define SENSING_THRESHOLD  0.85  // to normalize for different environments
#define DECISIVE_THRESHOLD 0.1  // decide whether  fish is inside
#define SENSE_TIME_GAP 5000     // this much of miliseconds between each sensing
#define SENSING_ITERATIONS 10  // how many sensing attempts at a one go

#define DIR 51   // DIR pin of the motor drive
#define STEP 50  // Step pin of the motor drive
#define ENBL 47  // En pin of the motor drive

#define IRsensor A0
#define CE 9
#define CSN 10

// motor one
#define ENA 7
#define IN1 6
#define IN2 5
// motor two
#define ENB 46
#define IN3 4
#define IN4 3

#define RANDOM_MOVE_TIME 10000
#define RANDOM_MOVE_TURNS 3
#define GET_STABLE_TIME 5000

//2 relay module
#define STEPPER_MOTOR 49
#define PROPELLER_MOTORS 48

// Don't change followings
#define MICROSTEPS 16
#define MOTOR_STEPS 200

//for the accelerometer
#define angleThresholdX 5
#define angleThresholdY 5
#define angleThresholdZ 10

//damage control
#define SOSDELAYTIME 1000

//keeps a count how many times the IR sensor went HIGH 
int fishCount = 0; //how many triggers for the IR sensor
int caughtfishflag = 0; //enough fish caught or not
int timetomove = 0; //no of times that the bot have stayed idle
int botmoved = 0; //bot is moved or not
int damage = 0; //damage control state
int initAccelerometer = 0;

//for the accelerometer
int16_t accY, accZ, accX;
float currentAngleX, currentAngleZ, currentAngleY,prevAngleX, prevAngleY, prevAngleZ;

//keeps prev value of IRsensor reading
int IRprev = 500;

//functions
void checkIRSensor();
void sendMessage();
int receiveState();
void random_move();
void start_slider();
void accelerometerCheck();


// CE, CSN pins
RF24 radio(9, 10);
MPU6050 mpu;
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, ENBL);

void setup(void){
  // set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stepper.begin(10, MICROSTEPS);

  pinMode(STEPPER_MOTOR, OUTPUT);
  pinMode(PROPELLER_MOTORS, OUTPUT);
  digitalWrite(STEPPER_MOTOR, HIGH);
  digitalWrite(PROPELLER_MOTORS, HIGH);
  
  while(!Serial);
  Serial.begin(9600);

  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  const uint64_t pipe = (0xE8E8F0F0E1LL);
  radio.openReadingPipe(1, pipe);

  radio.enableDynamicPayloads();
  radio.powerUp();

  mpu.initialize();
  mpu.setYAccelOffset(1593);
  mpu.setZAccelOffset(963);
  
}

int  mode = INIT_FISHING;

void loop() {

  switch (mode) {

    //-------------------------------------------------------------------------------------------------------------- INIT_FISHING
    case INIT_FISHING:
      botmoved = 0;
      caughtfishflag = 0;
      mode = DETECT_FISH;
      break;
    //-------------------------------------------------------------------------------------------------------------- DETECT_FISH
    case DETECT_FISH:
      accelerometerCheck();
      if (damage == 1){
        mode = DAMAGE_CONTROL;
        break;
      }
      checkIRSensor();
      //Serial.println(damage);
      //Serial.println(fishCount);
      if(fishCount > SENSING_ITERATIONS*DECISIVE_THRESHOLD){
        Serial.println("caught fish");
        caughtfishflag = 1;
        mode = START_SLIDER;
      }
      else{
        mode = RELAY_CONTACT;
      }
      break;
    //-------------------------------------------------------------------------------------------------------------- START_SLIDER
    case START_SLIDER:
      start_slider();
      fishCount =0;
      timetomove = 0;
      mode = RELAY_CONTACT;
      break;
      //-------------------------------------------------------------------------------------------------------------- RELAY_CONTACT
    case RELAY_CONTACT:
      //3
      sendMessage();
      Serial.println(timetomove);
      timetomove++;
      if(timetomove == RANDOM_MOVE_TURNS){
       mode = RANDOM_MOVE;
       break; 
      }
      mode = INIT_FISHING;

      break;
      //-------------------------------------------------------------------------------------------------------------- RANDOM_MOVE
    case RANDOM_MOVE:
      //4
      random_move();
      timetomove = 0;
      sendMessage();
      mode = INIT_FISHING;

      break;

      //-------------------------------------------------------------------------------------------------------------- DAMAGE_CONTROL
    case DAMAGE_CONTROL:
      sendMessage();
      delay(SOSDELAYTIME);
      break;
      
  }
}

void sendMessage(){
  //String receivedMessage = "";
 // unsigned int sizeofReceived = 32;
 // char receivedMessage[32] = {0};
//  char text[] = ""+"Yo wassup, haha";
  char botID[] = "00000001";
  char sendingMsg[32] = {0};
  char fishflag[2];
  itoa(caughtfishflag, fishflag, 10);
  char GPSLocation[] = "00000000000000000000";
  char movedToNewLocation[2];
  itoa(botmoved, movedToNewLocation, 10);
  char DamageControlState[2];
  itoa(damage, DamageControlState, 10);
  char ignorebit = "0";
  strcat(sendingMsg, botID);
  strcat(sendingMsg, fishflag);
  strcat(sendingMsg, GPSLocation);
  strcat(sendingMsg, movedToNewLocation);
  strcat(sendingMsg, DamageControlState);
  strcat(sendingMsg, ignorebit);

  

  radio.write(sendingMsg, sizeof(sendingMsg));
  //delay(100);
  radio.write(sendingMsg, sizeof(sendingMsg));
  //delay(100);
  radio.write(sendingMsg, sizeof(sendingMsg));
  Serial.println(sendingMsg);
}




void checkIRSensor() {
  int i;
  for(i = 0; i < 5; i++){
    int IRreading = analogRead(IRsensor);
    Serial.print(IRreading);
    Serial.print(":");
    Serial.println(SENSING_THRESHOLD * IRprev);
    if(SENSING_THRESHOLD * IRprev > IRreading){
        fishCount++;
        //Serial.println(fishCount);
    } 
    else{
      //IRprev = IRreading;
    }
    delay(2000); 
  }
}


void random_move(){
  Serial.println("random_moving");

  digitalWrite(PROPELLER_MOTORS, LOW);
    // turn on motor A
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    // turn on motor B
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 50);
    analogWrite(ENB, 50);
    delay(RANDOM_MOVE_TIME);
    // now turn off motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);  
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(PROPELLER_MOTORS, HIGH);
    delay(GET_STABLE_TIME);
    botmoved = 1;
    //initAccelerometer = 0;
}

void start_slider(){
    digitalWrite(STEPPER_MOTOR, LOW);
    Serial.println("forward sliding");
    
    // Move forward 200 steps
    stepper.rotate(200);
    delay(2000);

  // Move backward 200 steps
    Serial.println("reverse sliding");
    stepper.rotate(-200);
    delay(2000);

    digitalWrite(STEPPER_MOTOR, HIGH);
}

void accelerometerCheck(){
   // read acceleration and gyroscope values
  accY = mpu.getAccelerationY();
  accZ = mpu.getAccelerationZ();  
  accX = mpu.getAccelerationX();  
  currentAngleX = atan2(accY, accZ)*RAD_TO_DEG;
  currentAngleY = atan2(accZ, accX)*RAD_TO_DEG;
  currentAngleZ = atan2(accY, accX)*RAD_TO_DEG;

  if(currentAngleX - prevAngleX > angleThresholdX || currentAngleY - prevAngleY > angleThresholdY || currentAngleZ - prevAngleZ > angleThresholdZ){
    //danger state
    if(initAccelerometer != 0){
      damage = 1;
      Serial.println("danger");
    }  
    initAccelerometer = 1;
  }
  prevAngleX = currentAngleX;
  prevAngleY = currentAngleY;
  prevAngleZ = currentAngleZ;
  
 
  Serial.print("AngleX ");Serial.print(currentAngleX);
  Serial.print(" AngleY ");Serial.print(currentAngleY);
  Serial.print(" AngleZ ");Serial.print(currentAngleZ);
//  Serial.print("currentAngle ");Serial.print(currentAngle);
//  Serial.print(" error ");Serial.print(error);
  Serial.println();
  delay(1000);
  
  
}




