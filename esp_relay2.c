/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

//#ifdef ESP32
#include <WiFi.h>
#include <ESP32Ping.h>
#include <HTTPClient.h>
//#else
//  #include <ESP8266WiFi.h>
//  #include <ESP8266HTTPClient.h>
//  #include <WiFiClient.h>
//#endif

#include <ArduinoJson.h>

int succes_API = 0;
int requested_API = 0;

// Replace with your network credentials
//const char* ssid     = "Redmi Note 9 Pro";
const char* ssid     = "BARUNASTRA";
const char* password = "khususNALA";
//const char* password = "szberdua";

const int relay_ac_pin = 5;
const int relay_dc_pin = 4;

float current_measured = 0;

bool pv_on_switch = false;
bool using_pv = false;
int jam_on = 7;
int menit_on = 30;
int jam_off = 15;
int menit_off = 30;

int jam = 0;  
int menit = 0; 
int detik = 0;

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "zidan123123";

// REPLACE with your Domain name and URL path or IP address with path
String espName = "magiccom";
const char* serverName = "http://kawalgenteng.com/coba_quer.php";
String httpRequestData = "api_key=" + apiKeyValue + "&nama_esp=" + String(espName) + "";
//const char* httpRequestData = "api_key=zidan123123&nama_esp=magiccom";






int http_error_counter = 0;

void connectToWifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  http_error_counter = 0;
}

void setup() {
  Serial.begin(115200);
  connectToWifi();
  pinMode(relay_ac_pin, OUTPUT);
  pinMode(relay_dc_pin, OUTPUT);

  
}

void disablePVUsingPLN(){
  digitalWrite(relay_ac_pin, LOW);
//    delay(500);
  digitalWrite(relay_dc_pin, LOW);
}

bool checkConnToServer(){
  bool ret = Ping.ping("www.kawalgenteng.com");
//  int avg_time_ms = Ping.averageTime();
  Serial.print("RET PING ");
  Serial.println(ret);
//  Serial.print("LAT ");
//  Serial.println(avg_time_ms);
  return ret;
}

void loop() {
  //Check WiFi connection status
  IPAddress ip (100, 24, 25, 1); // The remote ip to ping
  
  if(WiFi.status()== WL_CONNECTED && (http_error_counter <= 30)){
    HTTPClient http;
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.setTimeout(350);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // Prepare your HTTP POST request data // untuk post esp name to get relay command and time


    // Send HTTP POST request
    Serial.println("POSTING TO SERVER");
   
   int httpResponseCode = http.POST(httpRequestData);
   String payload = http.getString(); 
   Serial.println(payload);
//   while (payload.length() <= 1) {
//    httpResponseCode = http.POST(httpRequestData);
//    payload = http.getString(); 
//   }

//   int httpResponseCode2 = http.POST(httpRequestData2);

    if (httpResponseCode>0) {
      succes_API += 1;
      http_error_counter = 0;
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      
      DynamicJsonDocument root(1024);
      deserializeJson(root, payload);
      // Parameters
      int relay_ac = root["relay_ac"]; 
      int relay_dc = root["relay_dc"]; 
      jam = root["jam"];  
      menit = root["menit"]; 
      detik = root["detik"];
      // Output to serial monitor
      Serial.print("AC:");
      Serial.println(relay_ac);
      Serial.print("DC:");
      Serial.println(relay_dc);
      Serial.print("jam:"); 
      Serial.println(jam);
      Serial.print("menit:"); 
      Serial.println(menit);
      Serial.print("detik:"); 
      Serial.println(detik);
  
      if(relay_dc == 2){
        if (jam >= jam_off) pv_on_switch = 0;
        else if (jam >= jam_on) pv_on_switch = 1;  
      }else if (relay_dc == 1) pv_on_switch = 1;
      else if (relay_dc == 0) pv_on_switch = 0;
  
      if(pv_on_switch) {
        digitalWrite(relay_ac_pin, HIGH);
  //      delay(1000);
        digitalWrite(relay_dc_pin, HIGH);
        using_pv = true;
      }else{
        disablePVUsingPLN();
        using_pv = false;
      }
  
      if(relay_ac == 0) digitalWrite(relay_ac_pin, HIGH);
     
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      http_error_counter += 1;
    }

    // Free resources
    http.end();
  }
  else {
    disablePVUsingPLN();
    Serial.println("WiFi Disconnected -- USING PLN");
    connectToWifi();
  }

  Serial.print("SUCCES API = ");
  Serial.println(succes_API);
  Serial.print("requested_API = ");
  requested_API++;
  Serial.println(requested_API);
  
  //Send an HTTP POST request every 3 seconds
  delay(500);  
}