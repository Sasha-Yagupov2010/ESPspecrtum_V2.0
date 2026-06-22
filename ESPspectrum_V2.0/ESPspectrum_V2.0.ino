// Прототипы функций из Disk_Driver.ino
bool check_os();
void saveData(String filename, String name, String value);
String readLine(String filename, int lineNumber);
String readValue(String filename, String name);
void writeFile(String filename, String data);
String readFile(String filename);
int get_line_count(String filename);
void appendFile(String filename, String data);

// Прототипы функций из Keyboard.ino
void setup_keyboard();
String get_key();

// Прототипы функций из Core.ino
void osLoop();
void run_cmd(String cmd);
void run_script_file(String path);
int splitCommand(String input, String* result, int maxParts);
String removeQuotes(String str);
bool isValidInt(String str);
bool isValidFloat(String str);
void handleVGACommand(String* parts, uint8_t count);
void handleSetCommand(String* parts, uint8_t count);
void handleMemoryCommand(String* parts, uint8_t count);
void free_print();

// Прототипы функций из Speacers.ino
void setup_speaker(int pin);
void create_tone(int pin, int frequency);
void create_tone(int pin, int frequency, int duration);
void disable_tone(int pin);

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

#define CLOCK 16 //D-
#define DATA 17  //D+
#define SD_PIN 5

const int SPEAKER_PIN = 25;  // Выберите подходящий пин для динамика

const bool developMode = true;

//VGA Device
VGA3Bit vga;

/*==============================================*/
#include <SD.h>
File myFile;

void setup(){
  Serial.begin(115200);
  setup_keyboard();
  
  vga.init(vga.MODE320x240, redPin, greenPin, bluePin, hsyncPin, vsyncPin);
  vga.setFont(CodePage437_8x16);
  
  vga.setCursor(2, 5);  vga.println("Starting...");
  vga.setCursor(2, 25); vga.println("ESPspectrum V2.0");
  vga.setFont(CodePage437_8x14);
  vga.setCursor(2, 100); vga.println("created on 2026");
  
  if(!developMode) delay(5000);
  
  vga.clear(vga.RGB(0, 0, 0));
  delay(1000);
  vga.setCursor(0, 0);

  if(!check_os()) return;
  delay(100);
}

void loop(){
  Serial.println("enter loop");
  osLoop();
  return;
}
