#include<SPI.h>
#include<RF24.h>
#include <string.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

// Modes
enum {RANDOM_MOVE,INIT_FISHING,DETECT_FISH,START_SLIDER,REVERSE_SLIDER,RELAY_CONTACT,ACK_LISTEN};

#define SENSING_THRESHOLD  0.7  // to normalize for different environments
#define DECISIVE_THRESHOLD 0.7  // decide whether  fish is inside
#define SENSE_TIME_GAP 5000     // this much of miliseconds between each sensing
#define SENSING_ITERATIONS 1  // how many sensing attempts at a one go


//accelerometer
#define angleThresholdX 5
#define angleThresholdY 5
#define angleThresholdZ 10

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
#define ENB 4
#define IN3 3
#define IN4 2

#define RANDOM_MOVE_TIME 10000
#define GET_STABLE_TIME 5000

//encryption
//  0001    0001        0001      0001        0001           0001 
// "botID-dangerState-lowPower-fishDetected-movedTonewPlace-ignore"
char plainText[32] = "00010000000000000000000";
char xorKey[11] = "ABCDEFGHIJHKLMNOPQRSTUVWXYZABCDEFGH";
  for(int x=0; x<10; x++)
  {
    string[x]=string[x]^key[x];
  }

MPU6050 mpu;
int16_t accY, accZ, accX;
float currentAngleX, currentAngleZ, currentAngleY,prevAngleX, prevAngleY, prevAngleZ;

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

// CE, CSN pins
RF24 radio(9, 10);

void setup(void){
  // initialize the MPU6050 and set offset values
  mpu.initialize();
  mpu.setYAccelOffset(1593);
  mpu.setZAccelOffset(963);
  //mpu.setXAccelOffset(1593);


  // set all the motor control pins to outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(POWER_MOTOR,OUTPUT);
  digitalWrite(POWER_MOTOR, LOW);

  while(!Serial);
  Serial.begin(9600);

  //communication
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  const uint64_t pipe = (0xE8E8F0F0E1LL);
  radio.openReadingPipe(1, pipe);

  radio.enableDynamicPayloads();
  radio.powerUp();
  
}

int  mode = INIT_FISHING;

void loop() {
  // read acceleration and gyroscope values
  accY = mpu.getAccelerationY();
  accZ = mpu.getAccelerationZ();  
  accX = mpu.getAccelerationX();  
  currentAngleX = atan2(accY, accZ)*RAD_TO_DEG;
  currentAngleY = atan2(accZ, accX)*RAD_TO_DEG;
  currentAngleZ = atan2(accY, accX)*RAD_TO_DEG;

  if(currentAngleX - prevAngleX > angleThresholdX || currentAngleY - prevAngleY > angleThresholdY || currentAngleZ - prevAngleZ > angleThresholdZ){
    //danger state
    Serial.println("danger");
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




  switch (mode) {

    //-------------------------------------------------------------------------------------------------------------- INIT_FISHING
    case INIT_FISHING:
      IRprev = 1000;
      fishCount =0;
      mode = DETECT_FISH;
      break;
    //-------------------------------------------------------------------------------------------------------------- DETECT_FISH
    case DETECT_FISH:
      checkIRSensor();
      if(fishCount > SENSING_ITERATIONS*DECISIVE_THRESHOLD){
        mode = START_SLIDER;
      }
      break;
    //-------------------------------------------------------------------------------------------------------------- START_SLIDER
    case START_SLIDER:
      if(moveSlider(true)){
        mode = REVERSE_SLIDER;
      }
      else{
        //  1
      }

      break;
      //-------------------------------------------------------------------------------------------------------------- REVERSE_SLIDER
    case REVERSE_SLIDER:
      if(moveSlider(false)){
        mode = RELAY_CONTACT;
      }
      else{
        //  2
      }

      break;
      //-------------------------------------------------------------------------------------------------------------- RELAY_CONTACT
    case RELAY_CONTACT:
      //3
      sendState(1);    
      mode = ACK_LISTEN;

      break;
      //-------------------------------------------------------------------------------------------------------------- RANDOM_MOVE
    case RANDOM_MOVE:
      //4
      random_move();
      mode = INIT_FISHING;

      break;
        //-------------------------------------------------------------------------------------------------------------- ACK_LISTEN
    case ACK_LISTEN:
      //5
      mode = receiveState();
      break;

  }
}



int sendState(int mode){
  //String receivedMessage = "";
  unsigned int sizeofReceived = 32;
  char receivedMessage[32] = {0};
//  char text[] = ""+"Yo wassup, haha";

  char sendingMsg[32] = {0};
  char modes[1] ;   
  itoa(mode, modes, 10);
  char botCredentials[] = "b1time0000";
  strcat(sendingMsg, modes);
  strcat(sendingMsg, botCredentials);


  radio.write(sendingMsg, sizeof(sendingMsg));
  delay(100);
  radio.write(sendingMsg, sizeof(sendingMsg));
  delay(100);
  radio.write(sendingMsg, sizeof(sendingMsg));
  Serial.println("We sent our message.");
  unsigned int j =0;
  while(j++<10){
    radio.startListening();
    Serial.println("Starting loop. Radio on.");
  
    if(radio.available()){
      radio.read(receivedMessage, sizeof(receivedMessage));
      //radio.read(receivedMessage, sizeofReceived);
      Serial.println(receivedMessage);
      Serial.println("Turning off the radio.");
      radio.stopListening();
      String stringMessage(receivedMessage);
      //delay(100);
      unsigned int nextMode = atoi(receivedMessage[31]);
      Serial.println(stringMessage);
      //return nextMode;
      return stringMessage.toInt();
    }  
    delay(1000);
  }
  
  return INIT_FISHING;   
}

int receiveState(){

  radio.startListening();
    Serial.println("Starting loop. Radio on.");
    char receivedMessage[32] = {0};
    if(radio.available()){
      radio.read(receivedMessage, sizeof(receivedMessage));
      Serial.println(receivedMessage);
      Serial.println("Turning off the radio.");
      radio.stopListening();
    }
    return INIT_FISHING;
  
}






void checkIRSensor() {
  int IRreading = analogRead(IRsensor);
  if(SENSING_THRESHOLD * IRprev > IRreading){
      fishCount++;
    } 
    else{
      IRprev = IRreading;
    }
}


bool moveSlider(bool dir) {
  if(dir)
    Serial.println("moving slider - pushing"); 
  else
    Serial.println("moving slider - pulling");
  return true; 
}




void random_move(){
  digitalWrite(POWER_MOTOR,HIGH);
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
    delay(GET_STABLE_TIME);
    digitalWrite(POWER_MOTOR,LOW);
}



