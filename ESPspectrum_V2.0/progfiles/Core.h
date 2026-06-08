#define drawmenufile "/Graphick/draw_menu.txt"

void run_cmd(string cmd){
	Serial.println(cmd);
}



void osLoop(){
	bool OS_run = true;
	while(OS_run){
		vga.setCursor(0,0);
		//menu
		uint_8 items = get_line_count(drawmenufile);
		for(int i=0; i<items;i++){
			String line = readLine(drawmenufile,i);
			vga.println(line);
		}

		if(get_key()!="none"){
			run_cmd(get_key())
		}

	}

}
