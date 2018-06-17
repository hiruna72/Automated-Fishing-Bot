
// TRANSMITTER CODE

//NOTE :- THIS CODE IS USED WHEN YOU WANT TO CONTROL THE ROBOT VIA THE COMPUTER!!!!

#include <VirtualWire.h>

void setup()
{
    Serial.begin(9600);    // Debugging only
    Serial.println("setup");

    // Initialise the IO and ISR
     vw_set_rx_pin(11);
    vw_setup(2000);  // Bits per sec
        vw_set_tx_pin(12); //Transmitter Data Pin to Digital Pin 3
}//close setup

void loop()
{
  
  if (Serial.available() > 0)
  {
     int sendByte = Serial.read();//Starts reading data from the serial monitor once condition is met
     
 
  switch (sendByte){
 
    case 'C':  //if the controller type f
  {
    
    char *msg2 = "C";
    const char *msg = "hello";

    digitalWrite(13, true); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    Serial.println("sent");
    digitalWrite(13, false);
    
    break;
     
  }
    case 'S':  //if controller types b
    {
   char *msg2 = "S";
   digitalWrite(13, true); // Flash a light to show transmitting
   vw_send((uint8_t *)msg2, strlen(msg2));//send byte to the receiver
   vw_wait_tx(); // Wait until the whole message is gone
   digitalWrite(13, false);  
   break;
    }
    
    case 'A': //if controller types s
    {
   char *msg2 = "A";
   digitalWrite(13, true); // Flash a light to show transmitting
   vw_send((uint8_t *)msg2, strlen(msg2));//send byte to the receiver
   vw_wait_tx(); // Wait until the whole message is gone
   digitalWrite(13, false);  
   break;
    }
    
    case 'l': //if controller types l
  {  
   char *msg2 = "l";
   digitalWrite(13, true); // Flash a light to show transmitting
   vw_send((uint8_t *)msg2, strlen(msg2));//send byte to the receiver
   vw_wait_tx(); // Wait until the whole message is gone
   digitalWrite(13, false);    
   break;
  }     
    case 'r': //if controller types r
    {
   char *msg2 = "r";
   digitalWrite(13, true); // Flash a light to show transmitting
   vw_send((uint8_t *)msg2, strlen(msg2));//send byte to the receiver
   vw_wait_tx(); // Wait until the whole message is gone
   digitalWrite(13, false);   
   break;
    }
    default://if any other value is entered
    {
      
       Serial.println("wrong entry");//print wrong entry in the serial monitor
    }
   }// close switch-case
  }//close if 
  for(int i=0;i<10;i++){
    listenForACK();
    delay(100);    
  }

 }//close loop

void listenForACK(){
  //Serial.println("checking");
   uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
  int i;
  for (i = 0; i < buflen; i++)
  {
      char a =buf[i];
      Serial.print(a);
      Serial.print(" ");
  }
  Serial.println();
}
}
  
 // End Of Code 
