
bool sendState(int mode){

	  const char text[] = "Yo wassup, haha";
  	radio.write(text, sizeof(text));
  	Serial.println("We sent our message.");
  	return true;
  
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
    return 6;
  
}



// void loop(void){
//   radio.startListening();
//   Serial.println("Starting loop. Radio on.");
//   char receivedMessage[32] = {0};
//   if(radio.available()){
//     radio.read(receivedMessage, sizeof(receivedMessage));
//     Serial.println(receivedMessage);
//     Serial.println("Turning off the radio.");
//     radio.stopListening();

//     String stringMessage(receivedMessage);

//     if(stringMessage == "GETSTRING"){
//       Serial.println("Looks like they want a string!");
//       const char text[] = "Yo wassup, haha";
//       radio.write(text, sizeof(text));
//       Serial.println("We sent our message.");
//     }
//   }
//   delay(100);

// }







