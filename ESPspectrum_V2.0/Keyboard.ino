const char keymap[] = {
  0, 0,  0,  0,  0,  0,  0,  0,
  0, 0,  0,  0,  0,  0, '`', 0,
  0, 0 , 0 , 0,  0, 'q', '1', 0,
  0, 0, 'z', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'y', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '/', 'l', ';', 'p', '-', 0,
  0, 0, '\'', 0, '[', '=', 0, 0,
  0, 0, 13, ']', 0, '\\', 0, 0,
  0, 0, 0, 0, 0, 0, 127, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', 0, 0,
  0, '+', '3', '-', '*', '9', 0, 0,
  0, 0, 0, 0
};

// Переменные для отслеживания состояния
volatile bool newKeyPressed = false;
volatile uint8_t lastScanCode = 0;
volatile uint8_t bitCount = 0;
volatile uint32_t scanVal = 0;
volatile uint32_t lastClockTime = 0;

// Обработчик прерывания по спадающему фронту CLOCK
void IRAM_ATTR onClockFall() {
  uint32_t now = micros();
  
  // Антидребезг
  if (now - lastClockTime < 50) return;
  lastClockTime = now;
  
  // Читаем данные на спадающем фронте
  bool dataBit = digitalRead(DATA);
  
  if (bitCount == 0 && dataBit == 0) {
    // Стартовый бит
    bitCount++;
    scanVal = 0;
  } else if (bitCount > 0 && bitCount < 9) {
    // Биты данных (1-8)
    scanVal |= (dataBit << (bitCount - 1));
    bitCount++;
  } else if (bitCount == 9) {
    // Паритетный бит (игнорируем)
    bitCount++;
  } else if (bitCount == 10) {
    // Стоповый бит
    if (dataBit == 1) {
      // Корректный пакет
      lastScanCode = scanVal;
      newKeyPressed = true;
    }
    bitCount = 0;
    scanVal = 0;
  } else {
    // Сброс при ошибке
    bitCount = 0;
    scanVal = 0;
  }
}

String get_key() {
  static bool keyPressed = false;
  static uint8_t lastKey = 0;
  
  if (newKeyPressed) {
    newKeyPressed = false;
    
    uint8_t scancode = lastScanCode;
    
    // Обработка отпускания клавиши (код 0xF0)
    if (scancode == 0xF0) {
      keyPressed = false;
      return "none";
    }
    
    // Игнорируем специальные коды
    if (scancode == 0xE0 || scancode == 0xF0) {
      return "none";
    }
    
    // Проверяем границы массива
    if (scancode < sizeof(keymap)) {
      char key = keymap[scancode];
      
      // Если это новая клавиша (не повтор)
      if (!keyPressed || lastKey != scancode) {
        keyPressed = true;
        lastKey = scancode;
        
        // Преобразуем символ в строку
        if (key == 13) return "enter";
        if (key == 127) return "backspace";
        if (key == ' ') return "space";
        if (key == 0) return "none";
        
        // Для обычных символов
        String result = "";
        result += key;
        return result;
      }
    }
  }
  
  return "none";
}

// Функция для настройки в setup()
void setup_keyboard() {
  pinMode(CLOCK, INPUT_PULLUP);
   pinMode(DATA, INPUT_PULLUP);
  
  // Настраиваем прерывание на спадающий фронт
  attachInterrupt(digitalPinToInterrupt(CLOCK), onClockFall, FALLING);
  
  // Инициализируем переменные
  bitCount = 0;
  scanVal = 0;
  newKeyPressed = false;
  lastClockTime = micros();
}
