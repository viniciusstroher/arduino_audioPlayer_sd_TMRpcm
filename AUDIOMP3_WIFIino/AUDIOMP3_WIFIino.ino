
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

void loop(){  
   
    uint8_t buffer[100] = {0};
    static uint8_t mux_id = 0; 
    
    if (wifi.createTCP(mux_id,HOST_NAME, HOST_PORT)) {
      char* params = "HOST CONNECT";
      uint32_t len  = wifi.recv(mux_id,buffer, sizeof(buffer), 100);
      
      if(len >0){
        Serial.println("len:"+String(len)); 
        String str = (char*)buffer; 
        //Serial.println("BUFFER: "+str);
        
        if(str.equals("start")){  
          SD.remove("fala.wav");
          
          char* params = "START";
        }else if(str.equals("stop")){
          char* params = "STOP";
        }else{
           
           File myFile = SD.open("fala.wav", FILE_WRITE);
           if (myFile) {
              for(uint32_t i = 0; i < len; i++) {
               myFile.write(buffer[i]); 
              }
              myFile.close();
           }
          
          /*PLAY*/
          
          //SD.open("fala.wav", FILE_WRITE);
          
          char* params = "CHUNK";
        }
        
        Serial.println(params);
        wifi.send(mux_id,(uint8_t)atoi(params), sizeof(params));
    }
   
    if(wifi.releaseTCP(mux_id)){ 
      Serial.println("OK released tcp");
    }
    //delay(5000);
  }
}

