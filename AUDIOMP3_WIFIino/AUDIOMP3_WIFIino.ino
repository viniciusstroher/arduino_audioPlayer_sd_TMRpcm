
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
 
 if(wifi.disableMUX()){
   Serial.println("Mux desabilitado");    
 }
 
}

void loop(){  
    uint8_t buffer[128] = {0};
    
    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
      
      Serial.println("HOST CONNECT");
      uint32_t len  = wifi.recv(buffer, sizeof(buffer), 10000);
      
      if(len >0){
         /*for(uint32_t i = 0; i < len; i++) {
           Serial.write((char)buffer[i]); 
          }*/
         Serial.println("Recebendo audio");
         
      }
    }
    
    if(wifi.releaseTCP()){ 
      Serial.println("OK released tcp");
    }

    delay(5000);
}

