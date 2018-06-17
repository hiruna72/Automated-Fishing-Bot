// CE, CSN pins
RF24 radio(9, 10);

void setup(void){
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

  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  const uint64_t pipe = (0xE8E8F0F0E1LL);
  radio.openReadingPipe(1, pipe);

  radio.enableDynamicPayloads();
  radio.powerUp();
  
}