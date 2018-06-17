void checkIRSensor() {
  int IRreading = analogRead(IRsensor);
  if(SENSING_THRESHOLD * IRprev > IRreading){
      fishCount++;
    } 
    else{
      IRprev = IRreading;
    }
}
