#include <ESP32Lib.h>
#include <Ressources/Font6x8.h>
#include <Ressources/CodePage437_8x8.h>
#include <Ressources/CodePage437_8x14.h>
#include <Ressources/CodePage437_8x16.h>

//pin configuration
const int redPin = 33;
const int greenPin = 14;
const int bluePin = 12;
const int hsyncPin = 2;
const int vsyncPin = 4;

//VGA Device
VGA3Bit vga;

/*==============================================*/
#include <SD.h>
File myFile;

void void setup(){
    Serial.begin(115200);
  vga.init(vga.MODE320x240, redPin, greenPin, bluePin, hsyncPin, vsyncPin);

  vga.setFont(CodePage437_8x16);
  vga.setCursor(2, 5);
  vga.println("Starting...");
  vga.setCursor(2, 25);
  vga.println("ESPspectrum V2.0");
  vga.setFont(CodePage437_8x14);
  vga.setCursor(2, 100);
  vga.println("created on 2023");
  delay(5000);
  vga.clear(vga.RGB(0, 0, 0));
  delay(1000);
  vga.setCursor(0, 0);

    if(!SD.begin(5)){
    vga.println("Card Mount Failed!"); Serial.println("Card Mount Failed!");
    delay(2000); vga.clear(vga.RGB(0, 0, 0)); vga.setCursor(0, 0); return;
  }
 
  delay(100);
}

void loop(){

}