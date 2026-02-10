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


  if (!SD.exists("/Scripts/") or !SD.exists("/Memory/")) {
    vga.println("Os error"); 
    Serial.println("Os error");
    delay(2000); 
    vga.clear(vga.RGB(0, 0, 0)); 
    vga.setCursor(0, 0); 
    return false;
  }


    return true;
}


void saveData(String filename, String name, String value){
  // Сначала читаем весь файл
  String fileContent = "";
  String newContent = "";
  bool nameFound = false;
  
  if(SD.exists(filename)) {
    File dataFile = SD.open(filename, FILE_READ);
    if(dataFile) {
      while(dataFile.available()) {
        String line = dataFile.readStringUntil('\n');
        line.trim();
        
        if(line.length() > 0) {
          // Проверяем, начинается ли строка с нужного имени
          if(line.startsWith(name + " ")) {
            // Заменяем старую запись на новую
            newContent += name + " " + value + "\n";
            nameFound = true;
          } else {
            newContent += line + "\n";
          }
        }
      }
      dataFile.close();
    }
  }
  
  // Если имя не найдено, добавляем новую запись
  if(!nameFound) {
    newContent += name + " " + value + "\n";
  }
  
  File dataFile = SD.open(filename, FILE_WRITE);
  if(dataFile) {
    dataFile.print(newContent);
    dataFile.close();
    Serial.println("Data saved: " + name + " = " + value);
  } else {
    Serial.println("Error opening file for writing: " + filename);
  }
}

String readLine(String filename, int lineNumber){
  File dataFile = SD.open(filename, FILE_READ);
  String line = "";
  int currentLine = 0;
  
  if(dataFile) {
    while(dataFile.available() && currentLine <= lineNumber) {
      line = dataFile.readStringUntil('\n');
      line.trim();
      if(currentLine == lineNumber) {
        dataFile.close();
        return line;
      }
      currentLine++;
    }
    dataFile.close();
  }
  
  return ""; // Возвращаем пустую строку, если строка не найдена
}

// Чтение значения по имени
String readValue(String filename, String name){
  File dataFile = SD.open(filename, FILE_READ);
  
  if(dataFile) {
    while(dataFile.available()) {
      String line = dataFile.readStringUntil('\n');
      line.trim();
      
      if(line.startsWith(name + " ")) {
        // Извлекаем значение после имени и пробела
        String value = line.substring(name.length() + 1);
        dataFile.close();
        return value;
      }
    }
    dataFile.close();
  }
  
  return ""; // Возвращаем пустую строку, если имя не найдено
}

// Запись данных в файл (простая перезапись)
void writeFile(String filename, String data){
  File dataFile = SD.open(filename, FILE_WRITE);
  if(dataFile) {
    dataFile.print(data);
    dataFile.close();
    Serial.println("File written: " + filename);
  } else {
    Serial.println("Error opening file: " + filename);
  }
}

// Чтение всего файла
String readFile(String filename){
  String content = "";
  File dataFile = SD.open(filename, FILE_READ);
  
  if(dataFile) {
    while(dataFile.available()) {
      content += (char)dataFile.read();
    }
    dataFile.close();
  }
  
  return content;
}

// Дополнительная функция: добавление данных в конец файла
void appendFile(String filename, String data){
  File dataFile = SD.open(filename, FILE_APPEND);
  if(dataFile) {
    dataFile.print(data);
    dataFile.close();
    Serial.println("Data appended to: " + filename);
  } else {
    Serial.println("Error opening file for append: " + filename);
  }
}
