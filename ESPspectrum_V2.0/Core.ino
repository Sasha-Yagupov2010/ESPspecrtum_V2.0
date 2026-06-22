//#define drawmenufile "/Graphick/draw_menu.txt"
#define drawmenuscript "/Scripts/draw_menu.speca"
#define MAX_PARTS 10

// Глобальные переменные для конфигурации
String romFilePath = "/Memory/rom.txt";  // Путь к ROM файлу по умолчанию
bool scriptRunning = false;              // Флаг выполнения скрипта

// ============================================
// Улучшенная функция splitCommand
// ============================================
int splitCommand(String input, String* result, int maxParts) {
    input.trim();
    int count = 0;
    int start = 0;
    int end = 0;
    bool inQuotes = false;
    
    for(int i = 0; i < input.length() && count < maxParts; i++) {
        char c = input[i];
        
        if(c == '\'') {
            if(!inQuotes) {
                // Начало кавычек
                if(i > start) {
                    // Сохраняем предыдущую часть если есть
                    result[count] = input.substring(start, i);
                    count++;
                }
                start = i + 1;  // Пропускаем кавычку
                inQuotes = true;
            } else {
                // Конец кавычек
                result[count] = "'" + input.substring(start, i) + "'";
                count++;
                start = i + 1;
                inQuotes = false;
            }
        }
        else if(c == ' ' && !inQuotes) {
            if(i > start) {
                result[count] = input.substring(start, i);
                count++;
            }
            start = i + 1;
        }
        
        // Последний символ
        if(i == input.length() - 1 && i >= start) {
            if(inQuotes) {
                result[count] = "'" + input.substring(start);
            } else {
                result[count] = input.substring(start);
            }
            count++;
        }
    }
    
    return count;
}

// ============================================
// Вспомогательные функции
// ============================================
String removeQuotes(String str) {
    if(str.startsWith("'") && str.endsWith("'")) {
        return str.substring(1, str.length() - 1);
    }
    return str;
}

bool isValidInt(String str) {
    if(str.length() == 0) return false;
    
    int start = 0;
    if(str[0] == '-') {
        if(str.length() == 1) return false;
        start = 1;
    }
    
    for(int i = start; i < str.length(); i++) {
        if(!isdigit(str[i])) return false;
    }
    return true;
}

bool isValidFloat(String str) {
    if(str.length() == 0) return false;
    
    int start = 0;
    bool hasDot = false;
    
    if(str[0] == '-') {
        if(str.length() == 1) return false;
        start = 1;
    }
    
    for(int i = start; i < str.length(); i++) {
        char c = str[i];
        if(c == '.') {
            if(hasDot) return false;  // Две точки
            hasDot = true;
        } else if(!isdigit(c)) {
            return false;
        }
    }
    return hasDot;  // Float должен иметь точку
}

// ============================================
// Обработчики команд по категориям
// ============================================

// Обработка VGA команд
void handleVGACommand(String* parts, uint8_t count) {
    if(count < 2) {
        Serial.println("Usage: vga [action] [params]");
        return;
    }
    
    String action = parts[1];
    
    if(action == "clear") {
        if(count == 2) {
            vga.clear(0);
            Serial.println("VGA cleared");
        } else {
            Serial.println("Usage: vga clear");
        }
    }
    else if(action == "setcursor") {
        if(count == 4) {
            int x = parts[2].toInt();
            int y = parts[3].toInt();
            vga.setCursor(x, y);
            Serial.println("Cursor set to: " + String(x) + "," + String(y));
        } else {
            Serial.println("Usage: vga setcursor [x] [y]");
        }
    }
    else if(action == "print" || action == "println") {
        if(count >= 4) {
            String type = parts[2];
            String value = parts[3];
            
            if(type == "text") {
                value = removeQuotes(value);
                if(action == "print") {
                    vga.print(value.c_str());
                } else {
                    vga.println(value.c_str());
                }
                Serial.println("Printed: " + value);
            }
            else if(type == "int") {
                int intValue = value.toInt();
                if(action == "print") {
                    vga.print(intValue);
                } else {
                    vga.println(intValue);
                }
                Serial.println("Printed int: " + String(intValue));
            }
            else if(type == "float") {
                float floatValue = value.toFloat();
                if(action == "print") {
                    vga.print(floatValue);
                } else {
                    vga.println(floatValue);
                }
                Serial.println("Printed float: " + String(floatValue));
            }
            else {
                Serial.println("Unknown type. Use: text, int, float");
            }
        } else {
            Serial.println("Usage: vga " + action + " [text|int|float] 'value'");
        }
    }
    else {
        Serial.println("Unknown vga action: " + action);
        Serial.println("Available: clear, print, println, setcursor");
    }
}

// Обработка SET команд
void handleSetCommand(String* parts, uint8_t count) {
    if(count < 3) {
        Serial.println("Usage: set [what] [value]");
        return;
    }
    
    String what = parts[1];
    String value = parts[2];
    /*
    if(what == "text") {
        if(count >= 4) {
            String subcmd = parts[2];
            String param = parts[3];

            
            if(subcmd == "color") {
                String color = removeQuotes(param);
                
                if(color == "white") vga.setTextColor(WHITE);
                else if(color == "black") vga.setTextColor(BLACK);
                else if(color == "red") vga.setTextColor(RED);
                else if(color == "green") vga.setTextColor(GREEN);
                else if(color == "blue") vga.setTextColor(BLUE);
                else if(color == "yellow") vga.setTextColor(YELLOW);
                else if(color == "cyan") vga.setTextColor(CYAN);
                else if(color == "magenta") vga.setTextColor(MAGENTA);
                else if(color.startsWith("#") && color.length() == 7) {
                    long colorValue = strtol(color.substring(1).c_str(), NULL, 16);
                    vga.setTextColor(colorValue);
                } else {
                    Serial.println("Unknown color. Use: white, black, red, green, blue, yellow, cyan, magenta, #RRGGBB");
                    return;
                }
                Serial.println("Text color set to: " + color);
            
            if(subcmd == "size") {
                int size = param.toInt();
                if(size > 0 && size <= 10) {
                    vga.setTextSize(size);
                    Serial.println("Text size set to: " + String(size));
                } else {
                    Serial.println("Invalid text size (1-10)");
                }
            }
            else {
                Serial.println("Usage: set text [color|size] [value]");
            }
        } else {
            Serial.println("Usage: set text [color|size] [value]");
        }
    }
    else {
        Serial.println("Unknown set command: " + what);
        Serial.println("Available: text");
    }
    */
}

// Обработка MEMORY команд
void handleMemoryCommand(String* parts, uint8_t count) {
    if(count < 2) {
        Serial.println("Usage: memory [action] [params]");
        return;
    }
    
    String action = parts[1];
    
    // memory delete 'filename'
    if(action == "delete" && count == 3) {
        String filename = removeQuotes(parts[2]);
        if(SD.exists(filename)) {
            SD.remove(filename);
            Serial.println("File deleted: " + filename);
        } else {
            Serial.println("File not found: " + filename);
        }
    }
    // memory createFile 'filename'
    else if(action == "createFile" && count == 3) {
        String filename = removeQuotes(parts[2]);
        File file = SD.open(filename, FILE_WRITE);
        if(file) {
            file.close();
            Serial.println("File created: " + filename);
        } else {
            Serial.println("Failed to create file: " + filename);
        }
    }
    // memory readline linenumber 'filename'
    else if(action == "readline" && count == 4) {
        int lineNumber = parts[2].toInt();
        String filename = removeQuotes(parts[3]);
        
        File file = SD.open(filename, FILE_READ);
        if(file) {
            int currentLine = 1;
            String lineContent = "";
            
            while(file.available() && currentLine <= lineNumber) {
                lineContent = file.readStringUntil('\n');
                lineContent.trim();
                if(currentLine == lineNumber) {
                    Serial.println("Line " + String(lineNumber) + ": " + lineContent);
                    break;
                }
                currentLine++;
            }
            
            if(currentLine < lineNumber) {
                Serial.println("Line " + String(lineNumber) + " not found");
            }
            
            file.close();
        } else {
            Serial.println("File not found: " + filename);
        }
    }
    // memory set romfile '/path/file.txt'
    else if(action == "set" && count == 4 && parts[2] == "romfile") {
        String filename = removeQuotes(parts[3]);
        if(SD.exists(filename)) {
            romFilePath = filename;
            Serial.println("ROM file set to: " + filename);
        } else {
            Serial.println("File not found: " + filename);
        }
    }
    // memory rom save type 'name' value
    else if(action == "rom") {
        if(count >= 4 && parts[2] == "save") {
            if(count == 6) {
                String type = parts[3];
                String name = removeQuotes(parts[4]);
                String value = parts[5];
                
                if(type == "string") {
                    value = removeQuotes(value);
                }
                
                // Используем твои функции
                saveData(romFilePath, name, value);
                Serial.println("Saved " + type + " '" + name + "' = " + value + " to ROM");
            } else {
                Serial.println("Usage: memory rom save [int|float|string] 'name' [value]");
            }
        }
        else if(count >= 4 && parts[2] == "read") {
            if(count == 5) {
                String type = parts[3];
                String name = removeQuotes(parts[4]);
                
                String value = readValue(romFilePath, name);
                if(value != "") {
                    Serial.println(name + " = " + value);
                } else {
                    Serial.println(name + " not found in ROM");
                }
            } else {
                Serial.println("Usage: memory rom read [int|float|string] 'name'");
            }
        }
        else {
            Serial.println("Usage: memory rom [save|read] [type] 'name' [value]");
        }
    }
    // memory list [directory]
    else if(action == "list") {
        String dirPath = "/";
        if(count == 3) {
            dirPath = removeQuotes(parts[2]);
        }
        
        File root = SD.open(dirPath);
        if(root) {
            Serial.println("Files in " + dirPath + ":");
            File entry = root.openNextFile();
            while(entry) {
                if(entry.isDirectory()) {
                    Serial.print("  [DIR] ");
                } else {
                    Serial.print("  [FILE] ");
                }
                Serial.println(entry.name());
                entry.close();
                entry = root.openNextFile();
            }
            root.close();
        } else {
            Serial.println("Failed to open directory: " + dirPath);
        }
    }
    else {
        Serial.println("Unknown memory action: " + action);
        Serial.println("Available: delete, createFile, readline, set, rom, list");
    }
}

// ============================================
// Основная функция run_cmd
// ============================================
void run_cmd(String input) {
    Serial.println("> " + input);
    
    String parts[MAX_PARTS];
    uint8_t count = splitCommand(input, parts, MAX_PARTS);
    
    if(count == 0) return;
    
    String command = parts[0];
    
    // Команда help
    if(command == "help" || command == "?") {
        Serial.println("=== Available Commands ===");
        Serial.println("vga clear                    - Clear screen");
        Serial.println("vga setcursor x y            - Set cursor position");
        Serial.println("vga print text 'text'        - Print text");
        Serial.println("vga println int value        - Print line with integer");
        Serial.println("set text color 'color'       - Set text color");
        Serial.println("set text size number         - Set text size (1-10)");
        Serial.println("memory delete 'file'         - Delete file");
        Serial.println("memory createFile 'file'     - Create empty file");
        Serial.println("memory readline n 'file'     - Read line n from file");
        Serial.println("memory set romfile 'path'    - Set ROM file path");
        Serial.println("memory rom save type 'n' v   - Save to ROM");
        Serial.println("memory rom read type 'n'     - Read from ROM");
        Serial.println("memory list [dir]            - List files");
        Serial.println("run 'script'                 - Run script file");
        Serial.println("stop                         - Stop current script");
        Serial.println("exit                         - Exit OS loop");
        Serial.println("===========================");
        return;
    }
    
    // Команда run для запуска скриптов
    if(command == "run" && count == 2) {
        String scriptPath = removeQuotes(parts[1]);
        run_script_file(scriptPath);
        return;
    }
    
    // Команда stop для остановки скрипта
    if(command == "stop") {
        scriptRunning = false;
        Serial.println("Script execution stopped");
        return;
    }
    
    // Команда exit для выхода из OS loop
    if(command == "exit") {
        // Это будет обработано в osLoop
        return;
    }
    
    // Основные команды
    if(command == "vga") {
        handleVGACommand(parts, count);
    }
    else if(command == "set") {
        handleSetCommand(parts, count);
    }
    else if(command == "memory") {
        handleMemoryCommand(parts, count);
    }
    else {
        Serial.println("Unknown command: " + command);
        Serial.println("Type 'help' for available commands");
    }
}

// ============================================
// Функции работы со скриптами
// ============================================
void run_script_file(String path) {
    
    scriptRunning = true;
    Serial.println("Running script: " + path);
    
    int items = get_line_count(path);  
    for(int i = 0; i < items && scriptRunning; i++) {
        String line = readLine(path, i);
        line.trim();
        
        // Пропускаем пустые строки и комментарии
        if(line.length() == 0 || line.startsWith("//") || line.startsWith("#")) {
            continue;
        }
        
        // Специальная команда для паузы
        if(line.startsWith("delay ")) {
            String delayStr = line.substring(6);
            int delayTime = delayStr.toInt();
            if(delayTime > 0) {
                Serial.println("Delay: " + String(delayTime) + "ms");
                delay(delayTime);
            }
            continue;
        }
        
        // Специальная команда для вывода
        if(line.startsWith("echo ")) {
            String message = line.substring(5);
            if(message.startsWith("'") && message.endsWith("'")) {
                message = message.substring(1, message.length() - 1);
            }
            Serial.println("[SCRIPT] " + message);
            continue;
        }
        
        // Выполнение обычной команды
        run_cmd(line);
        delay(10);  // Небольшая пауза между командами
    }
    
    scriptRunning = false;
    Serial.println("Script finished: " + path);
}

void free_print(){
  vga.clear(0);
  vga.setCursor(0,0);
  
  while(true)
  {
    String key = get_key();
    
    if(key!="none")
    {
      if(key=="backspace"){break;}
      vga.print(key.c_str());
    }
  } 
}

void osLoop(){
    bool OS_run = true;
    while(OS_run){
        run_script_file(drawmenuscript);

        String key = get_key();
        if(key != "none"){
            if(key==1){free_print();}
            else if(key==2){}
            else if(key==5){command_loop();}
            else{Serial.println("Wrong comand");}
        }
        
        delay(50);  
    }
}
