
//analog pin 0
//IR sensor 
// value is positive if fish is present
int IRsensor = A0; 

//keeps a count how many times the IR sensor went HIGH 
int fishCount =0;

//factor to mark a threshold for fish presence
float fishDetectFactor = 0.75;

//keeps prev value of IRsensor reading
int IRprev = 1000;

//whenever the IR sensor reads fish presence blink
int fishDetect = 8;

void setup(){
pinMode(fishDetect, OUTPUT);
Serial.begin(9600);
digitalWrite(fishDetect, LOW);
}

void loop(){
  detectFishandCount();
  delay(50
  
}
boolean detectFishandCount(){
  
  while(count--){
     int IRreading = analogRead(IRsensor);
    
    Serial.println(IRreading);
    if(fishDetectFactor * IRprev > IRreading){
      digitalWrite(fishDetect, HIGH);
      Serial.println("inside");
    } 
    else{
      IRprev = IRreading;
    }
    delay(1000); //just here to slow down the output for easier reading
    digitalWrite(fishDetect, LOW);
  }
  count = 10;
}

