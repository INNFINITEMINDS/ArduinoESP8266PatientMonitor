#include <stdio.h>
#define ONE_WIRE_BUS 6
#include<OneWire.h>
#include<DallasTemperature.h>
char temp_buffer[25];
byte maxsensors = 0;
 
/* DS18S20 Temperature chip i/o */
OneWire  ds(ONE_WIRE_BUS);
DallasTemperature sensors(&ds);

void scanSensors(void)
{
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  while (ds.search(addr))
  {
    //Serial.print("R=");
    for( i = 0; i < 8; i++) {
      //Serial.print(addr[i], HEX);
      //Serial.print(" ");
    }
  
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        //Serial.print("CRC is not valid!\n");
        return;
    }
    
    if ( addr[0] == 0x10) {
        //Serial.print("Device is a DS18S20 family device.\n");
        maxsensors++;
    }
    else {
      if (addr[0] == 0x28) {
        //Serial.print("Device is a DS18B20 family device.\n");
        maxsensors++;
      }
      else {
        //Serial.print("Device is unknown!\n");
        //Serial.print("Device ID: ");
        //Serial.print(addr[0],HEX);
        //Serial.println();
        return;
      }
    }
    // The DallasTemperature library can do all this work for you!
    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end
    delay(800);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad
    //Serial.print("P=");
    //Serial.print(present,HEX);
    //Serial.print(" ");
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
      //Serial.print(data[i], HEX);
      //Serial.print(" ");
    }
    //Serial.print(" CRC=");
    //Serial.print( OneWire::crc8( data, 8), HEX);
    //Serial.println();
  }
  //Serial.print("No more addresses.\n");
  ds.reset_search();
  delay(250);
}

void init_temp_sensors() {
  sensors.begin();
  scanSensors();
  Serial.print("Found");
  Serial.print("    ");
  Serial.print(maxsensors);
  Serial.print("    ");
  Serial.println("DS1820 Temperature Sensors");
}

float get_temp() {
  float average=0;
  sensors.requestTemperatures(); 
  for (int i=0;i<maxsensors;i++)
  {
    float f = sensors.getTempCByIndex(i); 
    //Serial.print("Sensor ");
    //Serial.print(i,DEC);
    //Serial.println(dtostrf(f, 6, 2, temp_buffer));
    average +=f;
  }
  return average/maxsensors;
}

