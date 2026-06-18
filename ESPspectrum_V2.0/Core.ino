//#define drawmenufile "/Graphick/draw_menu.txt"
#define drawmenuscript "/Scripts/draw_menu.speca"
#define MAX_PARTS 10

// Функция возвращает количество найденных частей
int splitCommand(String input, String* result, int maxParts) {
    input.trim();
    int count = 0;
    int start = 0;
    int end = 0;
    
    while (count < maxParts - 1 && start < input.length()) {
        // Пропускаем пробелы
        while (start < input.length() && input[start] == ' ') {
            start++;
        }
        
        if (start >= input.length()) break;
        
        // Проверяем кавычки
        if (input[start] == '\'') {
            // Текст в кавычках
            end = start + 1;
            while (end < input.length() && input[end] != '\'') {
                end++;
            }
            if (end < input.length()) {
                result[count] = input.substring(start, end + 1);
                count++;
                start = end + 1;
            } else {
                // Незакрытая кавычка - берем до конца
                result[count] = input.substring(start);
                count++;
                break;
            }
        } else {
            // Обычное слово
            end = input.indexOf(' ', start);
            if (end == -1) {
                result[count] = input.substring(start);
                count++;
                break;
            } else {
                result[count] = input.substring(start, end);
                count++;
                start = end + 1;
            }
        }
    }
    
    // Последняя часть, если осталась
    if (count < maxParts && start < input.length()) {
        result[count] = input.substring(start);
        count++;
    }
    
    return count;
}

void run_cmd(String input) {
    Serial.println("Input: " + input);
    
    String parts[MAX_PARTS];
    uint8_t count = splitCommand(input, parts, MAX_PARTS);

    String command, action1, action2, arg1, arg2, arg3;

    //обрабатываем корготкие комманды
    if(count==2){ 
      command = parts[0];
      action1 = parts[1];

      if(command=="vga"){
        if(action=="clear")vga.clear(0);
        }
        else if(){}
        else{Serial.println("Unknown command"); return;}
    }
    
    else if(count==4){
      
    }
    else{
      Serial.println("Unknown command");
      return;
    }

    
}

void run_script_file(String path){
   int items = get_line_count(path);  
   for(int i=0; i<items;i++){
       String line = readLine(path,i);
       
       if(line=="" || line==" " ||line=="/n") continue;
       run_cmd(line);
   }
}


void osLoop(){
    bool OS_run = true;
    while(OS_run){
        run_script_file(drawmenuscript);

        String key = get_key();
        if(key != "none"){
            run_cmd(key);  
        }
        
        delay(50);  
    }
}
