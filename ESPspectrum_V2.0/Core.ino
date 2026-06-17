#define drawmenufile "/Graphick/draw_menu.txt"

void run_cmd(String cmd){
    Serial.println(cmd);
}

void osLoop(){
    bool OS_run = true;
    while(OS_run){
//        vga.clear(vga.RGB(0, 0, 0)); 
        vga.setCursor(0,0);
        
        //menu
        int items = get_line_count(drawmenufile);  
        for(int i=0; i<items;i++){
            String line = readLine(drawmenufile,i);
            vga.println(line.c_str());
        }

        String key = get_key();
        if(key != "none"){
            run_cmd(key);  
        }
        
        delay(50);  
    }
}
