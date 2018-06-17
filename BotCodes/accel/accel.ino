#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

#define angleThresholdX 5
#define angleThresholdY 5
#define angleThresholdZ 10

MPU6050 mpu;


int16_t accY, accZ, accX;

float currentAngleX = angleThresholdX + 1; 
float currentAngleY = angleThresholdY + 1; 
float currentAngleZ = angleThresholdZ + 1; 

float prevAngleX = angleThresholdX;
float prevAngleY = angleThresholdY;
float prevAngleZ = angleThresholdZ;

void setup() {
  Serial.begin(115200);
  // set the status LED to output mode 
  //pinMode(13, OUTPUT);
  // initialize the MPU6050 and set offset values
  mpu.initialize();
  mpu.setYAccelOffset(1593);
  mpu.setZAccelOffset(963);
  //mpu.setXAccelOffset(1593);

}


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
  delay(1000);
  
  
}
