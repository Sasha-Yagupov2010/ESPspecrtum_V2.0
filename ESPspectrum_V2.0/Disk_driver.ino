bool check_os(){
  if(!SD.begin(SD_PIN))
  {
    vga.println("Card Mount Failed!"); 
    Serial.println("Card Mount Failed!");
    delay(2000); 
    vga.clear(vga.RGB(0, 0, 0)); 
    vga.setCursor(0, 0); 
    return false;
  }



    return true;
}
