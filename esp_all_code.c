#include <LiquidCrystal_I2C.h>

//HARDWARE
const int lcdColumns = 16;
const int lcdRows = 2;
const int buttonPin[] = {27,14,25,26};
const int buzzer = 32;

//INTERFACE HARDWARE SOFTWARE
int buttonState = 0; 
int last_keypad = -1;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


//SOFTWARE
bool mode_auto = true;
short source_power = -1;
int time_pv_on = 8;
int time_pv_off = 16;
short min_countdown_override = 99;
short sec_countdown_override = 99;
short counter_1s_timer =0;
bool is_override_timeout = false;

//SERVER
String ssid_wifi = "BARUNASTRA";
String pass_wifi = "khususKAMI";
bool wifi_connected = false;
bool is_last_update_succes = false;
short override_min_timer = 60; //exclue PV time running
short jam_server = 10;
short menit_server = 10;
short detik_server = -1;

//LCD
short keypad_pressed = -1;
short lcd_display_mode = 0; //0 Default Home, 1 Yes No Question, 2 Default Overrideden
short confirm_yes_no = -1;

//prototipe
void buzzerBeep();

void LCDOverrideMode(){
  if(jam_server >= time_pv_on && jam_server < time_pv_off){
    min_countdown_override = (time_pv_off - jam_server) * 60 - menit_server;
  }

  lcd.setCursor(0, 0);
  // print message
  lcd.print("SRC ");
  
  if(wifi_connected)
    lcd.print("<");
  else
    lcd.print(" ");

  lcd.print("TIMER");

  if(is_last_update_succes)
    lcd.print(">");
  else
    lcd.print(" ");

  lcd.print(" MODE");
  lcd.setCursor(0,1);
  
  if(source_power == 1)
    lcd.print("PLN ");
  else if(source_power == 2)
    lcd.print("PV  ");
  else if(source_power == 0)
    lcd.print("OFF ");
  else
    lcd.print("FAIL");   
  lcd.print("");

  char waktu[6];
  sprintf(waktu, "%03d-%02d", min_countdown_override, sec_countdown_override);
  lcd.print(waktu);
  lcd.print("  ");
  
  lcd.print(" MAN");

  counter_1s_timer++;
  if(counter_1s_timer >=6){
    sec_countdown_override--;
    if(sec_countdown_override < 0){
      sec_countdown_override = 59;
      min_countdown_override--;
      if(min_countdown_override<0)
        is_override_timeout = true;
    }

    counter_1s_timer = 0; 
  }
    
  
}

void LCDHomeMode(){
  lcd.setCursor(0, 0);
  // print message
  lcd.print("SRC ");
  
  if(wifi_connected)
    lcd.print("<");
  else
    lcd.print(" ");

  lcd.print("TIMER");

  if(is_last_update_succes)
    lcd.print(">");
  else
    lcd.print(" ");

  lcd.print(" MODE");
  
  lcd.setCursor(0,1);
  
  if(source_power == 1)
    lcd.print("PLN ");
  else if(source_power == 2)
    lcd.print("PV  ");
  else if(source_power == 0)
    lcd.print("OFF ");
  else
    lcd.print("FAIL");   
  lcd.print(" ");

  char waktu[6];
  sprintf(waktu, "%02d-%02d", time_pv_on, time_pv_off);
  lcd.print(waktu);
  lcd.print("  ");
  if(mode_auto)
    lcd.print("AUTO");
  else
    lcd.print(" MAN");
  
}

bool LCDQuestionYesNo(String msg){
//  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
  for(int i = 0; i < 16 - msg.length(); i++){
    lcd.print(" ");
  }
  lcd.setCursor(0,1);
  lcd.print("YES(1)     NO(3)");  
  confirm_yes_no = 0;
  return true;
}

void keypadInterupt(){
  Serial.println("INTERUPT");
  for(int x=0; x<4; x++)
  {
    //signifying the state of which the button is in by reading the appropriate pin #
    buttonState = digitalRead(buttonPin[x]);
    if (buttonState == LOW){
      Serial.print("PRESSED ");
      Serial.print(x+1);
      Serial.print(" ");
      Serial.println(buttonPin[x]);  
      if(x == 0 && last_keypad != 0){ //PLN ON, PV OFF
        last_keypad = 0;
        buzzerBeep();
        if(lcd_display_mode == 1){
          if(mode_auto){
            lcd_display_mode = 2; //berubah ke overridden
            mode_auto = false;
            min_countdown_override = override_min_timer;
            sec_countdown_override = 59;
            is_override_timeout = false;
          }else{
            lcd_display_mode = 0;
            mode_auto = true;
          }
          
        }
        if(lcd_display_mode == 2){
          //switch to PLN, PV OFF
          source_power = 1;
        }
        
      }

      if(x == 1 && last_keypad != 1){ //PLN OFF, PV ON
        last_keypad = 1;
        buzzerBeep();
        if(lcd_display_mode == 2){
          source_power = 2;
        }
      }
      
      if(x == 2 && last_keypad != 2){
        last_keypad = 2;
        //PLN OFF, PV ON
        if(lcd_display_mode == 1){
          if(mode_auto){
            lcd_display_mode = 0;
            mode_auto = true;
          }else{
            lcd_display_mode = 2;
            mode_auto = false;
          }
        }
      }
      if(x == 3 && last_keypad != 3){
        last_keypad = 3;
        
        //MODE SELECTION 
        lcd_display_mode = 1;
      }
    }
    
  }

}

void connectToWifi(){
  
}

void connToServer(){
  
}

void initBuzzer(){
  ledcSetup(0, 700, 8);
  ledcAttachPin(32, 0);
}

short buzzer_timer_counter = 0;
void buzzerStop(){
  buzzer_timer_counter = 0;
  ledcWrite(0, 0);
}

void buzzerBeep(){
  if(buzzer_timer_counter ==0)
    ledcWrite(0, 150);

  if(buzzer_timer_counter==1){
    ledcWrite(0, 0);
    buzzer_timer_counter = -1;
  }
    
  buzzer_timer_counter++;
}

void buzzerWarning(){
  if(buzzer_timer_counter ==0)
    ledcWrite(0, 150);

  if(buzzer_timer_counter==6)
    ledcWrite(0, 200);

  if(buzzer_timer_counter==7){
    ledcWrite(0, 0);
    buzzer_timer_counter = -3;
  }
    
  buzzer_timer_counter++;
}

void setup(){  
  Serial.begin(9600);  

  initBuzzer();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  //Keypad
  for(int x=0; x<4; x++)
  {
    pinMode(buttonPin[x], INPUT_PULLUP); 
    attachInterrupt(digitalPinToInterrupt(buttonPin[x]), keypadInterupt, CHANGE);
  } 


   
}

void loop(){
  //DISPLAY
  if(lcd_display_mode  == 0){
    LCDHomeMode();
    buzzerStop();
  }else if(lcd_display_mode == 1){
    buzzerWarning();
    if(mode_auto)
      LCDQuestionYesNo("GO MANUAL MODE!?");
    else
      LCDQuestionYesNo("GO AUTO MODE?");
  }else if(lcd_display_mode == 2){
    buzzerStop();
    LCDOverrideMode();  
  }

  //SERVER

  //SLEEP 10hz
  delay(100);
}
