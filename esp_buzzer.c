const int buzzer = 32; //buzzer to arduino pin 9

int freq = 2000;
int channel = 0;
int resolution = 8;

void setup(){
 
//  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
ledcSetup(channel, freq, resolution);
  ledcAttachPin(32, channel);
}

void loop(){

 ledcWrite(channel, 100);
 delay(700);
 ledcWrite(channel, 200);
 delay(100);
 ledcWrite(channel, 0);
 
 delay(1000);

 ledcWrite(channel, 100);
 delay(100);
 ledcWrite(channel, 200);
 delay(700);
 ledcWrite(channel, 100);
 delay(100);
 ledcWrite(channel, 0);
 delay(2000);
// for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle=dutyCycle+10){
//  
//    Serial.println(dutyCycle);
//  
////    ledcWrite(channel, dutyCycle);
//    delay(1000);
//  }
  
}