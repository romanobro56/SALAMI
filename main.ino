#include <Arduino.h>
#include <WiFiServer.h>
#include <SSD1306Wire.h>
#include "OpenThingsFramework.h"

#define WIFI_SSID "CICS_Makerspace" //change
#define WIFI_PASSWORD "makerspace" //maybe change

#define CLOUD_HOST "ws.cloud.openthings.io"
#define CLOUD_PORT 80
#define LOCAL_SERVER_PORT 80

// !!! Replace this by the unique token created for you !!!
#define CLOUD_TOKEN "OTd6aad91611ff59ff97f791e5c5ecdf"

// BluetoothSerial SerialBT;
// //define motor driver pins
#define IN1 14// motor 1 fwd pin
#define IN2 12 //motor 1 bck pin
#define IN3 13 //motor 2 fwd pin
#define IN4 15 //motor 2 bck pin
#define ENA A4 //motor 1 speed control
#define ENB A10 //motor 2 speed control

//define ultrasonic dist sensor pins
#define TRIG 5
#define ECHO 18

//distance queue size
#define DISTQ 10
#define STOPDIST 20


/*
- this array stores the DISTQ most recent distances recorded from ultrasonic dist sensor
- we'll use this as a circular queue to keep consistency
*/

double distances[DISTQ];

//in a stopped state, this will fill the circular queue with distances
void setDistArray(){
  for(int i = 0; i<DISTQ; i++){
    distances[i] = measureDistance();
  }
}

//ultrasonic distance sensor measure distance; taken from lecture slides (11 for reference)
double measureDistance(){
    // send a 10us pulse to trigger pin to start
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // wait till echo pin's rising edge
  while(digitalRead(ECHO)==LOW) { }
  unsigned long start_time = micros();
  // wait till echo pin's falling edge
  while((digitalRead(ECHO)==HIGH)) { }
  // calculate the time of flight in us
  unsigned long T = micros() - start_time;
  // convert time of flight to distance
  return T*0.01716f; // 34320cm / 2 / 10^6
}

//finds median in the circular queue
double getMedianDist(){
  double copy[DISTQ];
  for(int i = 0; i<DISTQ; i++){
    copy[i] = distances[i];
  }
  //sort list
  for(int i = 0; i < DISTQ; i++){
    int smallest = i;
    for(int j = i+1; j < DISTQ; j++){
      if(copy[j] < copy[smallest]){
        smallest = j;
      }
    }
    int temp = copy[i];
    copy[i] = copy[smallest];
    copy[smallest] = temp;
  }
  return copy[DISTQ/2];
}


/*
basic motor movement methods
from the back: motor1 is the right wheel, motor2 is the left wheel
*/
void motor1Bck(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
void motor1Fwd(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}
void motor2Fwd(){
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void motor2Bck(){
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//moves car forward
void moveForward(){
  motor1Fwd();
  motor2Fwd();
}

//moves car backward
void moveBackward(){
  motor1Bck();
  motor2Bck();
}

//stops car movement
void stopCar(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//turns right 90
void turnRight(){
  motor1Fwd();
  motor2Bck();
  delay(400);
  stopCar();
}

//turns left 90
void turnLeft(){
  motor1Bck();
  motor2Fwd();
  delay(400);
  stopCar();
}

//spins left 360
void spinLeft(){
  motor1Fwd();
  motor2Bck();
  delay(1200);
  stopCar();
}

//spins right 360
void spinRight(){
  motor2Fwd();
  motor1Bck();
  delay(1200);
  stopCar();
}

//turns right 180
void turn180(){
  motor1Bck();
  motor2Fwd();
  delay(660);
  stopCar();
}
//end of basic motor functions

//begin WiFi portion
void on_forward(const OTF::Request &req, OTF::Response &res){
    moveForward();
    delay(1000);
    stopCar();

    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
    Serial.println("received request");
}
void on_backward(const OTF::Request &req, OTF::Response &res){
    moveBackward();
    delay(1000);
    stopCar();

    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
}
void on_right(const OTF::Request &req, OTF::Response &res){
    turnRight();

    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
}
void on_left(const OTF::Request &req, OTF::Response &res){
    turnLeft();

    res.writeStatus(200, "OK");
    res.writeHeader(F("content-type"), F("text/html"));
    res.writeBodyChunk(F("{\"result\": \"success\"}"));
}
void on_180(const OTF::Request &req, OTF::Response &res){
  turn180();

  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html"));
  res.writeBodyChunk(F("{\"result\": \"success\"}"));
}
void on_360(const OTF::Request &req, OTF::Response &res){
  spinRight();

  res.writeStatus(200, "OK");
  res.writeHeader(F("content-type"), F("text/html"));
  res.writeBodyChunk(F("{\"result\": \"success\"}"));
}

OTF::OpenThingsFramework *otf;
//const char* deviceKey = "SALAMI";
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi network...");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.print("Connected. Local IP: ");
  Serial.println(WiFi.localIP());

  // Create OTC Object
  otf = new OTF::OpenThingsFramework(LOCAL_SERVER_PORT, CLOUD_HOST, CLOUD_PORT, CLOUD_TOKEN, false);
  otf->localServerBegin();  // start local server
  

  otf->on(F("/forward"), on_forward);
  otf->on(F("/backward"), on_backward);
  otf->on(F("/right"), on_right);
  otf->on(F("/left"), on_left);
  otf->on(F("/360"), on_360);
  otf->on(F("/180"), on_180);
  // analogWrite(ENA, 255);//sets speed to 255 (0-255 if using PWM)
  // analogWrite(ENB, 255);
}
//ENB and ENA motor speed can be set via analogWrite


int distIndex = 0;
void loop(void){
  otf->loop();
  // turnLeft();
  // // turnLeft();
  // stopCar();
  // delay(5000);
  // turnRight();
  // stopCar();
  // delay(5000);
  // spinRight();
  // delay(5000);
  // if(distIndex == 0){
  //   setDistArray();
  // }
  // distances[distIndex%DISTQ] = measureDistance();
  // distIndex++;
  // //if at any time, distance is too close, car will stop
  // // turnLeft();
  // // delay(500);
  // // turnRight();
  // // delay(2000);
  // if(getMedianDist() <= 20){
  //   Serial.println("stopping car, turning around");
  //   stopCar();
  //   delay(10000);
  //   // delay(1000);
  //   // moveBackward();
  //   // delay(500);
  //   //turn180();
  //   Serial.println("measuring in front");
  //   setDistArray();
  // }
  // Serial.println(getMedianDist());
  // //delay(5000);
  // //analogWrite(ENB, 255);

  // // analogWrite(ENA, 255);
  // // analogWrite(ENB, 255);
  // // moveForward();
  // // delay(2000);
  // // stopCar();
  // // delay(1000);
  // // // analogWrite(ENA, 255);
  // // // analogWrite(ENB, 255);
  // // moveBackward();
  // // delay(2000);
  // // stopCar();
  // // delay(5000);
}

// void loop() {
//     // Read audio data (either from I2S mic or WAV file)
//     uint8_t audio_buffer[512];
//     size_t bytes_read = 0;
    
//     i2s_read(I2S_NUM_0, audio_buffer, sizeof(audio_buffer), &bytes_read, portMAX_DELAY);
    
//     // Send via Bluetooth
//     if (bytes_read > 0) {
//         SerialBT.write(audio_buffer, bytes_read);
//     }
// }