
#include <SD.h>                 
#include <SPI.h>
#include <SoftwareSerial.h>
#include "ESP8266.h"

//ARDUINO UNO
#define SD_ChipSelectPin 4 
#define SSID        "w3GET"
#define PASSWORD    "minhavisita"

//#define SSID        "Venizao"
//#define PASSWORD    "venizao123"

#define HOST_NAME   "192.168.0.56"
#define HOST_PORT   8090

SoftwareSerial mySerial(3, 2); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

void setup(){ 

 Serial.begin(9600);

 if (!SD.begin(SD_ChipSelectPin)) {
   Serial.println("SD CARD ERROR !");
   return;
 }
 
 if(wifi.setOprToStation()){
   Serial.println("Setando cliente wifi");
 }
 
 if(wifi.joinAP(SSID, PASSWORD)){
   Serial.println(wifi.getLocalIP().c_str());    
 }
 
 if(wifi.enableMUX()){
   Serial.println("Mux habilitado");    
 }
 
}
int chunks = -1;
void loop(){  
   
    uint8_t buffer[128] = {0};
    static uint8_t mux_id = 0;
    
    if (wifi.createTCP(mux_id,HOST_NAME, HOST_PORT)) {
      
      Serial.println("HOST CONNECT");
      uint32_t len  = wifi.recv(mux_id,buffer, sizeof(buffer), 10000);
   
      if(len >0){
        String str = (char*)buffer;
        if(str.equals("start")){
          Serial.println("START RECORD AUDIO");
        }else if(str.equals("stop"){
          Serial.println("STOP RECORD AUDIO");
        }else{
          for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
          }
          Serial.println();
        }
        /*
        
        String paramName = str.substring(0,str.indexOf(":"));
        Serial.println(paramName);
        if(paramName.equals("start")){
          String paramValue = str.substring(str.indexOf(":")+1,-1);
          chunks = paramValue.toInt();
          
          if(chunks == 0){
            
          }else{
          
          }
          
        }
        
      }*/
    }
   
    if(wifi.send(mux_id,buffer, sizeof(buffer))){
      Serial.println("Enviando Status");
    }
    
    if(wifi.releaseTCP(mux_id)){ 
      Serial.println("OK released tcp");
    }

    //delay(5000);
}

