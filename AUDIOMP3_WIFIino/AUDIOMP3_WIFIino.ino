
#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4           //using digital pin 4 on arduino nano 328
#include<SPI.h>
#include <TMRpcm.h>                  //  also need to include this library...
 
TMRpcm tmrpcm;                       // create an object for use in this sketch

//ARDUINO UNO

#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "w3GET"
#define PASSWORD    "minhavisita"

SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

void setup(){ 
  
 Serial.begin(9600);
 Serial.print("setup begin\r\n");

 if (!SD.begin(SD_ChipSelectPin)) {
   Serial.println("initialization failed!");
   return;
 }
 
 if(wifi.joinAP(SSID, PASSWORD)){
   Serial.print("Join AP success\r\n");
   Serial.print("IP: ");
   Serial.println(wifi.getLocalIP().c_str());    
 }else{
   Serial.print("Join AP failure\r\n");
 }
    
 if (wifi.enableMUX()) {
   Serial.print("multiple ok\r\n");
 } else {
   Serial.print("multiple err\r\n");
 }
    
 if (wifi.startTCPServer(8090)) {
    Serial.print("start tcp server ok\r\n");
 } else {
    Serial.print("start tcp server err\r\n");
 }

 if (wifi.setTCPServerTimeout(10)) { 
    Serial.print("set tcp server timout 10 seconds\r\n");
 } else {
    Serial.print("set tcp server timout err\r\n");
 }
    
 /*tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
 tmrpcm.volume(1);
 tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset*/
 
 Serial.print("setup end\r\n");
}
 
void loop(){  
    uint8_t buffer[128] = {0};
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 1000);
    
    if (len > 0) {

        if (myFile) {
          Serial.print("Gravando Arquivo - INICIANDO");
          
          myFile.println(buffer);
          myFile.close();
          
          Serial.println("Gravando Arquivo - OK");
        }
        
        if(wifi.send(mux_id, buffer, len)) {
            Serial.println("send back - ok");
        } else {
            Serial.println("send back - err");
        }
        
        if (wifi.releaseTCP(mux_id)) {
            Serial.println("release tcp - ok");
        } else {
            Serial.println("release tcp - err");
        }
       
    }
} 
