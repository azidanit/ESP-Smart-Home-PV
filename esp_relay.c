/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-relay-module-ac-web-server/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

const int relay2 = 5;
const int relay1 = 4;

void setup() {
  Serial.begin(115200);
  pinMode(relay2, OUTPUT);
  pinMode(relay1, OUTPUT);
}

void loop() {
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  // digitalWrite(relay1, LOW);
  // delay(1000);
  // digitalWrite(relay2, LOW);
  // Serial.println("Current Flowing");
  // delay(5000); 
  
  // // Normally Open configuration, send HIGH signal stop current flow
  // // (if you're usong Normally Closed configuration send LOW signal)
  // digitalWrite(relay2, HIGH);
  // delay(1000);
  // digitalWrite(relay1, HIGH);
  // Serial.println("Current not Flowing");
  // delay(5000);
  bool relay_ac = false;
  bool relay_dc = false;

  Serial.print("AC ");
  Serial.println(relay_ac);
  
  Serial.print("DC ");
  Serial.println(relay_dc);

  delay(1000);
}