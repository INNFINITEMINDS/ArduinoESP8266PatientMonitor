#include<EEPROM.h>



class Wifi {
  private: 
  String _SSID , Password;  
  int addr=0; 
  boolean store(String _SSID , String Password)
  {
   EEPROM.write(addr,_SSID);
   addr ++;
   EEPROM.write(addr,Password); 
  }

  void EEPROM_init()
  {
  EEPROM.begin(512);
  } 
}

