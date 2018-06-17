
void random_move(){
	digitalWrite(POWER_MOTOR,HIGH);
  	// turn on motor A
  	digitalWrite(IN1, HIGH);
  	digitalWrite(IN2, LOW);
    // turn on motor B
  	digitalWrite(IN3, HIGH);
  	digitalWrite(IN4, LOW);
   	analogWrite(ENA, 150);
   	analogWrite(ENB, 150);
   	delay(RANDOM_MOVE_TIME);
  	// now turn off motors
  	digitalWrite(IN1, LOW);
  	digitalWrite(IN2, LOW);  
  	digitalWrite(IN3, LOW);
  	digitalWrite(IN4, LOW);
  	delay(GET_STABLE_TIME);
  	digitalWrite(POWER_MOTOR,LOW);
}
