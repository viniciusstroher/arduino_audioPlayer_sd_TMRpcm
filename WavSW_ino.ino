
#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328
#include<SPI.h>
#include <TMRpcm.h>           //  also need to include this library...
 
TMRpcm tmrpcm;   // create an object for use in this sketch

void setup(){
  Serial.begin(9600);
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
 
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("NO SD DETECTED");
  return;   // don't do anything more if not
  }
  Serial.println("Play audio");
 tmrpcm.volume(1);
 tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
}
 
void loop(){  
 
}
