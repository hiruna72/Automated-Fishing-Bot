#define reading A0
#define buzzer 12
unsigned int value;

void setup() {
// put your setup code here, to run once:
pinMode(reading,INPUT); //declare pin A0 as input
pinMode(buzzer,OUTPUT); //declare pin 12 as output
Serial.begin(9600); //set the baud rate of the serial monitor
Serial.print("Value");
Serial.println ("\tVoltage"); //”t” create spacing of a tab
}

void loop() {
// put your main code here, to run repeatedly:
value = analogRead(reading); //take reading from sensor
float raw_voltage = value*0.00488; //convert analog value to voltage
Serial.print(value); //display analog value in serial monitor
Serial.print("\t");
Serial.println(raw_voltage); //display value of voltage in serial monitor
delay(100); //create a delay of 0.1s
if(raw_voltage<1.7){ 
  digitalWrite(buzzer,HIGH);
  
}
else{
  digitalWrite(buzzer,LOW);
  
}
delay(500);
}
