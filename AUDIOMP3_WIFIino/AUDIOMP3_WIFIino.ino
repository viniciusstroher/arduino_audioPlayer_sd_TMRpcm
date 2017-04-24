
#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include<SPI.h>
#include <TMRpcm.h>           //  also need to include this library...
 
TMRpcm tmrpcm;   // create an object for use in this sketch
/*
	tem exemplo de uso do SD nos exemplos do arduino IDE
*/

#define SSID        "w3GET"
#define PASSWORD    "minhavisita"

//ARDUINO UNO
#include "ESP8266.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);

void setup(){ 
  //mySerial.begin(9600);
  Serial.begin(9600);
  Serial.print("setup begin\r\n");
  
  /* 
  if (wifi.setOprToStationSoftAP()) {
       Serial.print("to station + softap ok\r\n");
  } else {
        Serial.print("to station + softap err\r\n");
  }*/
 
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");
        Serial.println(wifi.getLocalIP().c_str());    
    } else {
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
    
    
    if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
      Serial.println("NO SD DETECTED");
      return;   // don't do anything more if not
    }
      /*tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
     
      
        // Serial.println("Play audio");
        tmrpcm.volume(1);
       // tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset*/
    Serial.print("setup end\r\n");
}
 
void loop(){  
    uint8_t buffer[128] = {0};
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
    if (len > 0) {
        Serial.print("Status:[");
        Serial.print(wifi.getIPStatus().c_str());
        Serial.println("]");
        
        Serial.print("Received from :");
        Serial.print(mux_id);
        Serial.print("[");
        for(uint32_t i = 0; i < len; i++) {
            Serial.print((char)buffer[i]);
        }
        Serial.print("]\r\n");
        
        if(wifi.send(mux_id, buffer, len)) {
            Serial.print("send back ok\r\n");
        } else {
            Serial.print("send back err\r\n");
        }
        
        if (wifi.releaseTCP(mux_id)) {
            Serial.print("release tcp ");
            Serial.print(mux_id);
            Serial.println(" ok");
        } else {
            Serial.print("release tcp");
            Serial.print(mux_id);
            Serial.println(" err");
        }
        
        Serial.print("Status:[");
        Serial.print(wifi.getIPStatus().c_str());
        Serial.println("]");
    }
} 
