void create_tone(int pin, int tone){
  tone(pin,tone);
}

void create_tone(int pin, int tone, int dur){
  tone(pin,tone,dur);
}
void disable_tone(int pin){
  noTone();
}