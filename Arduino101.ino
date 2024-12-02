#include <ESP32Servo.h>
#define SERVO1 23

Servo myservo;  // create a servo object
void setup() {
  myservo.attach(SERVO1);  // attaches servo object on pin 23
}
void loop() {
  for (int degree = 0; degree <= 180; degree+=1) {
    myservo.write(degree);  // tell servo to rotate to degree
    delay(15);  // waits 15ms for the servo to reach the position
  }
  for (int degree = 180; degree >= 0; degree -= 1) {
    myservo.write(degree); delay(15);
  }
}
