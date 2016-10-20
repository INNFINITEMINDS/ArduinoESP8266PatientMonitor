
void init_ecg() {
  pinMode(9, INPUT); // Setup for leads off detection LO +
  pinMode(8, INPUT); // Setup for leads off detection LO -
}

int fetch_ecg(boolean serialOn ){
  if((digitalRead(9) == 1)||(digitalRead(8) == 1)){
    if(serialOn)
    {
    Serial.println('!');
    }
    return 0;
  }
  else{
    unsigned int ecg=analogRead(A1);
    if(serialOn)
    {
    // send the value of analog input 0:
      Serial.println(ecg);
    }
    return ecg;
  }
  //Wait for a bit to keep serial data from saturating
  delay(1);
}
