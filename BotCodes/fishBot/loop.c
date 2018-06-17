int  mode;

void loop() {

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