#include<stdlib.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define mySSID "Tenda"
#define PASS "abbasi09"
String IP ="192.168.0.101";
int pid=7;
String GET;
String cmd;
SoftwareSerial _monitor(10,11); // RX, TX
char bufferCharemg[4];
char bufferCharecg[4];
char bufferCharpulse[4];
char bufferChartemp[4];
char bufferCharbpup[4];
char bufferCharbplw[4];
unsigned int timerLCD=0;
unsigned int bpTimer=0;
unsigned int pingTimer=0;
boolean state=true;
boolean pingRequested=false;
int bloodpressureup=0;
int bloodpressurelow=0;
int ecg= 0;
int pulse= 0;
int eeprom_pulse=0;
void setup()
{
  Wire.begin();
  randomSeed(A3);
  pinMode(A6, INPUT); 
  pinMode(7,OUTPUT);
  _monitor.begin(9600);
  Serial.begin(115200);
  init_OLED();
  print_text(0,18," Connecting.."," to Wifi"," ");
  sendDebug("AT");
  delay(3000);
  if(_monitor.find("OK"))
   {
    Serial.println("RECEIVED: OK");
    while(!connectWiFi())
    {
      
    }
    _monitor.println("AT+CWMODE=1");
    delay(500);//delay after mode change
    _monitor.println("AT+RST");
    print_text(0,18," Connected"," to Wifi","");  
    init_temp_sensors();
    init_ecg();
    timerLCD=millis();
    pingTimer=timerLCD;
   }
}
 

void loop(){
 int emg= analogRead(A6);
 get_pulse();
 analogWrite(7,map(emg,0,500,0,255));
 if (fetch_ecg(false) !=0)
 {
   ecg=fetch_ecg(false);
 }
 int temp= get_temp();
 if( millis() - timerLCD > 8000)
{
  timerLCD=millis();  
  char* emgString= itoa(emg,bufferCharemg,10);
  char* ecgString= itoa(ecg,bufferCharecg,10);
  char* pulseString= itoa(pulse,bufferCharpulse,10);
  char* tempString= itoa(temp,bufferChartemp,10);
  char* bpupString= itoa(bloodpressureup,bufferCharbpup,10);
  char* bplwString= itoa(bloodpressurelow,bufferCharbplw,10);
  if(state)
  {
  print_med(50,18,emgString,tempString,ecgString," EMG: " , " Temp: ", " ECG: ");
  }
  else
  {
  print_med(50,18,pulseString,bpupString,bplwString," BPM: " , " BPup: ", " BPLw ");
  }
  emgString="";
  ecgString="";
  pulseString="";
  tempString="";
  state=!state;
}

if( millis() - pingTimer >10000)
{
 pingTimer=millis(); 
 updateTemp(temp,ecg,emg,pulse,bloodpressureup,bloodpressurelow,pid);
}

if( millis() - bpTimer > 20000)
{
  bpTimer=millis();
  bloodpressureup=eeprom_i2c_read(0x50, 99);
  bloodpressurelow=eeprom_i2c_read(0x50,100);
  eeprom_pulse=eeprom_i2c_read(0x50, 101);
}

}

void updateTemp(int temp, int ecg , int emg , int pulse , int bphigh, int bplow, int pid){
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",8081";
  sendDebug(cmd);
  delay(800);
  GET = "GET http://192.168.1.3:8081/patientmonitoring/fill_sensor_data.php?ecg="+String(ecg)+"&emg="+String(emg)+"&bphigh="+String(bphigh)+"&pid="+String(pid)+"&bplow="+String(bplow)+"&pulse="+String(pulse)+"&temp="+String(temp);
  cmd = GET;
  cmd += " HTTP/1.0\r\n\r\n\r\n";
  //GET http://{domain + path} HTTP/1.0\r\n\r\n
  _monitor.print("AT+CIPSEND=");
  _monitor.println(cmd.length());
  if(_monitor.find(">")){
    Serial.print(">");
    _monitor.print(cmd);
    Serial.println(cmd);
  }else{
    sendDebug("AT+CIPCLOSE");
  }
  if(_monitor.find("OK")){
    Serial.println("RECEIVED: OK");
  }else{
    Serial.println("RECEIVED: Error");
  
  }
}
void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  _monitor.println(cmd);
} 
 
boolean connectWiFi(){
  _monitor.println("AT+CWMODE=1");
  delay(2000);
  cmd="AT+CWJAP=\"";
  cmd+=mySSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(2000);
  if(_monitor.find("OK")){
    Serial.println("RECEIVED: OK");
    return true;
  }else{
    Serial.println("RECEIVED: Error");
    return false;
  }
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
