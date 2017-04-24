
#include <SD.h>                 
#include<SPI.h>
#include <TMRpcm.h>
#include <SoftwareSerial.h>
#include "ESP8266.h"

//ARDUINO UNO
#define SD_ChipSelectPin 4 
#define SSID        "w3GET"
#define PASSWORD    "minhavisita"

SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

//TMRpcm tmrpcm;

File myFile;
int audios = 1;

uint8_t buffer[128] = {0};
uint8_t mux_id = 0;

String response_ok       = "OK";
char* response_ok_buffer = new char[response_ok.length()+1];



void setup(){ 
  
 Serial.begin(9600);
 if (!SD.begin(SD_ChipSelectPin)) {
   Serial.println("initialization failed!");
   return;
 }
 
 if(wifi.joinAP(SSID, PASSWORD)){
   Serial.println(wifi.getLocalIP().c_str());    
 }

 wifi.startTCPServer(8090);
 wifi.setTCPServerTimeout(10);
    
 /*tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
 tmrpcm.volume(1);
 tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset*/
 strncpy(response_ok_buffer, response_ok.c_str(), response_ok.length()+1);
 
 Serial.println("setup ok!");
}



void loop(){  
    
   
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    
    if (len > 0) {
        myFile = SD.open(audios+".wav", FILE_WRITE);
        
        if (myFile) {
          Serial.print("Gravando Arquivo");
         
          for(uint32_t i = 0; i < len; i++) {
           myFile.write(buffer[i]); 
          }

          myFile.close();
          audios +=1;
          
          wifi.send(mux_id, response_ok_buffer, sizeof(response_ok_buffer));

          if (wifi.releaseTCP(mux_id)) {
            Serial.println("release tcp - ok");
          } else {
              Serial.println("release tcp - err");
          }
          
        }

    }
} 
       

      
