// Для ESP32 нужно использовать ledc вместо tone()

// Настройка для одного пина
int speakerChannel = 0;
int speakerResolution = 8;

void setup_speaker(int pin) {
    ledcSetup(speakerChannel, 2000, speakerResolution);
    ledcAttachPin(pin, speakerChannel);
}

void create_tone(int pin, int frequency){
    ledcWriteTone(speakerChannel, frequency);
}

void create_tone(int pin, int frequency, int duration){
    ledcWriteTone(speakerChannel, frequency);
    delay(duration);
    ledcWriteTone(speakerChannel, 0);
}

void disable_tone(int pin){
    ledcWriteTone(speakerChannel, 0);  // Исправлено
}
