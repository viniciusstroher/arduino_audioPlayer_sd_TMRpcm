
#include <SD.h>                 
#include <SPI.h>
#include <SoftwareSerial.h>
#include "ESP8266.h"

//ARDUINO UNO
#define SD_ChipSelectPin 4 
//#define SSID        "w3GET"
//#define PASSWORD    "minhavisita"

#define SSID        "Venizao"
#define PASSWORD    "venizao123"

SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

int audios = 1;

void(* resetFunc) (void) = 0; 

void setup(){ 
 mySerial.begin(9600);
 Serial.begin(9600);
 
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
 wifi.setOprToStationSoftAP();
 if(wifi.joinAP(SSID, PASSWORD)){
   Serial.println(wifi.getLocalIP().c_str());    
 }
 
 if(wifi.enableMUX()){
   Serial.println("Mux on");    
 }else{
    Serial.println("Mux off");    
 }

 if (!SD.begin(SD_ChipSelectPin)) {
   Serial.println("SD CARD ERROR !");
   return;
 }
  
 Serial.println("setup ok!");
}

static uint8_t mux_id = 0;
#define HOST_NAME   "192.168.0.56"
#define HOST_PORT   (8090)

void loop(){  
    uint8_t buffer[128] = {0};
    uint8_t mux_id;
    
    if (wifi.createTCP(mux_id, HOST_NAME, HOST_PORT)) {
      Serial.print("HOST CONNECT");
      uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
      if(len >0){
         /*for(uint32_t i = 0; i < len; i++) {
           Serial.write((char)buffer[i]); 
          }*/
         Serial.println("Recebendo audio");
         File myFile = SD.open(audios+".wav", FILE_WRITE);
         if (myFile) {
            for(uint32_t i = 0; i < len; i++) {
             myFile.write(buffer[i]); 
            }
            audios +=1;
            myFile.close();
            
         }
      }
    }else{
      Serial.println("TCP ERROR");
      wifi.restart(); 
    }

    
     

    /*if (len > 0) {
        Serial.println("RECEIVE FROM SERVER");
        
        /*File myFile = SD.open(audios+".wav", FILE_WRITE);
        if (myFile) {
          Serial.print("Gravando Arquivo");
         
          for(uint32_t i = 0; i < len; i++) {
           myFile.write(buffer[i]); 
          }

          myFile.close();
           audios +=1;
          
           /*tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
           tmrpcm.volume(1);
           tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
        }*/
        
    //}
    
    /*if(wifi.send(mux_id, buffer, len)){
      Serial.println("OK send");
    }else{
      Serial.println("FAIL send");
    }*/
    
    if(wifi.releaseTCP(mux_id)){ 
      Serial.println("OK released tcp");
    }else{
       resetFunc();
       Serial.println("FAIL released tcp");
    }
}

