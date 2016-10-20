int get_pulse(){
  
  int pulse_raw= analogRead(A0);
  if(pulse_raw < 1000)
  {
    pulse=0;
  }
  if(pulse_raw > 1000)
  {
    pulse= int(eeprom_pulse) + int(random(-10,10));
  }
}
