#include "WifiCam.hpp"
#include <WiFi.h>

static const char* WIFI_SSID = "NANO";
static const char* WIFI_PASS = "11223344";

#define l_led 12
#define r_led 13


//// Set your Static IP address
//IPAddress local_IP(192, 168, 43, 3);
//// Set your Gateway IP address
//IPAddress gateway(192, 168, 43, 1);
//
//IPAddress subnet(255, 255, 255, 0);


esp32cam::Resolution initialResolution;

WebServer server(80);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(33, OUTPUT);
  pinMode(l_led, OUTPUT);
  pinMode(r_led, OUTPUT);
  
  digitalWrite(33, LOW);//on 33 led
  
  delay(2000);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Configures static IP address
//  if (!WiFi.config(local_IP, gateway, subnet)) {
//    Serial.println("STA Failed to configure");//error
//    
//    for(int i = 0 ; i < 50 ; i++){
//      digitalWrite(l_led, LOW);
//      digitalWrite(r_led, LOW);
//      delay(100);
//      digitalWrite(l_led, HIGH);
//      digitalWrite(r_led, HIGH);
//      delay(100);
//    }
//  }
  
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");            //connecting

      if(digitalRead(l_led) == 0){
        digitalWrite(l_led, HIGH);
        digitalWrite(r_led, LOW);
        delay(500);
      }
      else{
        digitalWrite(r_led, HIGH);
        digitalWrite(l_led, LOW);
        delay(500);
      }
  }
  
  Serial.println("WiFi connected");//alarm
  
  for(int i = 0 ; i < 10 ; i++){
      digitalWrite(l_led, LOW);
      digitalWrite(r_led, LOW);
      delay(250);
      digitalWrite(l_led, HIGH);
      digitalWrite(r_led, HIGH);
      delay(250);
    }

  {
    using namespace esp32cam;

    initialResolution = Resolution::find(1024, 768);

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
      Serial.println("camera initialize failure");//error
      
      for(int i = 0 ; i < 20 ; i++){
        digitalWrite(l_led, LOW);
        digitalWrite(r_led, LOW);
        delay(100);
        digitalWrite(l_led, HIGH);
        digitalWrite(r_led, HIGH);
        delay(100);
      }
      ESP.restart();
    }
    Serial.println("camera initialize success");//alarm
    
    digitalWrite(l_led, LOW);
    digitalWrite(r_led, LOW);
  }

  addRequestHandlers();
  server.begin();
}

void loop()
{
  server.handleClient();
}
