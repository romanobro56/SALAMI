// #include <Arduino.h>
// #include <WiFiServer.h>
// #include <SSD1306Wire.h>
// #include "OpenThingsFramework.h"

// #define WIFI_SSID "TMOBILE-18DD" //change
// #define WIFI_PASSWORD "lrtsucks" //maybe change

// #define CLOUD_HOST "ws.cloud.openthings.io"
// #define CLOUD_PORT 80
// #define LOCAL_SERVER_PORT 80

// // !!! Replace this by the unique token created for you !!!
// #define CLOUD_TOKEN "your_token"

// #define BUT0 34
// #define BUT1 0
// #define BUT2 35
// #define LED 2
// #define NEOP 25
// #define BUZZER 17
// #define LIGHT_SENSOR A3

// OTF::OpenThingsFramework *otf;
// SSD1306Wire lcd(0x3c, SDA, SCL);
// Adafruit_NeoPixel pixels = Adafruit_NeoPixel(256, NEOP, NEO_GRB+NEO_KHZ800);

// String getTouchPin();
// String getButtons();
// void playTune(char *notes);

// void on_homepage(const OTF::Request &req, OTF::Response &res) {
//   res.writeStatus(200, "OK");
//   res.writeHeader(F("content-type"), F("text/html"));  
//   res.writeBodyChunk(FPSTR(ui_html)); // return the raw string defined in html_index.h
// }



// void on_get(const OTF::Request &req, OTF::Response &res) {
//   char json[500];
//   sprintf(json, "{\"led\":%d,\"light\":%d,\"buttons\":\"%s\",\"touch\":\"%s\"}",
//     digitalRead(LED), analogRead(LIGHT_SENSOR), getButtons().c_str(), getTouchPin().c_str());
//   res.writeStatus(200, "OK");
//   res.writeHeader(F("content-type"), F("text/html"));
//   res.writeBodyChunk(json);
// } 

// void on_set(const OTF::Request &req, OTF::Response &res) {
//   char *type = req.getQueryParameter(F("type"));
//   char *value = req.getQueryParameter(F("value"));
//   if(!type || !value) {
//     res.writeStatus(200, "OK");
//     res.writeHeader(F("content-type"), F("text/html"));
//     res.writeBodyChunk(F("{\"result\": \"missing type and/or value parameters\"}"));
//   } else {
//     if(strcmp(type, "led")==0) {
//       if(value[0]=='0') digitalWrite(LED, 0);
//       else digitalWrite(LED, 1);
//     } else if(strcmp(type, "tune")==0) {
//       playTune(value);
//     } else if(strcmp(type, "msg")==0) {
//       lcd.clear();
//       lcd.drawString(0, 0, value);
//       lcd.display();
//     } else if(strcmp(type, "color")==0) {
//       Serial.println(value);
//       int c = strtoul(value, NULL, 16);
//       Serial.println(c);
//       for(int i=0;i<pixels.numPixels();i++) {
//         pixels.setPixelColor(i, c);
//       }
//       pixels.show();
//     }
//     res.writeStatus(200, "OK");
//     res.writeHeader(F("content-type"), F("text/html"));
//     res.writeBodyChunk(F("{\"result\": \"success\"}"));
//   }
// }

// // void setup(void){
// //   Serial.begin(115200);
// //   // Init buttons
// //   pinMode(BUT0, INPUT);
// //   pinMode(BUT1, INPUT);
// //   pinMode(BUT2, INPUT);
// //   pinMode(LED, OUTPUT);

// //   // Init buzzer
// //   pinMode(BUZZER, OUTPUT);
// //   digitalWrite(BUZZER, 0);
// //   ledcSetup(0, 5000, 8);
// //   ledcAttachPin(BUZZER, 0);

// //   // Init LCD
// //   lcd.init();
// //   lcd.setFont(ArialMT_Plain_16);
// //   lcd.flipScreenVertically();
// //   lcd.clear();
// //   lcd.display();

// //   // Init Neopixel
// //   pixels.begin();
// // 	pixels.setBrightness(32);
// //   pixels.clear();
// //   pixels.show();

// //   // Start WiFi
// //   WiFi.mode(WIFI_STA);
// //   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
// //   Serial.println("Connecting to WiFi network...");
// //   while (WiFi.status() != WL_CONNECTED) { delay(500); }
// //   Serial.print("Connected. Local IP: ");
// //   Serial.println(WiFi.localIP());

// //   // Create OTC Object
// //   otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);
// //   otf->localServerBegin();  // start local server

// //   // Set up server endpoints (APIs)
// //   otf->on(F("/"), on_homepage);
// //   otf->on(F("/get"), on_get);
// //   otf->on(F("/set"), on_set);
// // }
 
// // void loop(void){
// //   otf->loop();
// // }

// const byte TOUCH_PINS[] = {T0, T3, T4, T5, T6, T7, T8, T9};
// const char* TOUCH_NAMES[] = {"T0", "T3", "T4", "T5", "T6", "T7", "T8", "T9"};
// const byte numTouchPins = sizeof(TOUCH_PINS);
// const byte TOUCH_THRESHOLD = 20;

// String getTouchPin() {
//   for(byte i=0;i<numTouchPins;i++) {
//     if(touchRead(TOUCH_PINS[i]) < TOUCH_THRESHOLD) {
//       return TOUCH_NAMES[i];
//     }
//   }
//   return "-";
// }

// String getButtons() {
//   String ret = "";
//   if(digitalRead(BUT0)==0) ret+="B0 ";
//   if(digitalRead(BUT1)==0) ret+="B1 ";
//   if(digitalRead(BUT2)==0) ret+="B2 ";
//   return ret.length()==0 ? "-" : ret;
// }

// #define NOTE_DURATION 250
// const int notes_freq[] = {262, 294, 330, 349, 392, 440, 494};
// void playTune(char* notes) {
//   for(byte i=0;i<strlen(notes);i++) {
//     char c = notes[i];
//     if(c>='1' && c<='7') {
//       ledcWriteTone(0, notes_freq[c-'1']);
//       delay(NOTE_DURATION);
//       ledcWriteTone(0, 0);
//     } else {
//       delay(NOTE_DURATION);
//     }
//   }
//   digitalWrite(BUZZER, 0);
// }