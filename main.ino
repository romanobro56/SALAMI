#include <Arduino.h>
// #include "BluetoothSerial.h"
// #include <driver/i2s.h>
// const int sampleWindow = 50;
// #define AMP_PIN A4
// unsigned int sample;

// void setup()
// {
//   Serial.begin(9600);
// }

// void loop()
// {
//   unsigned long startMillis = millis(); // Start of sample window
//   unsigned int peakToPeak = 0;   // peak-to-peak level

//   unsigned int signalMax = 0;
//   unsigned int signalMin = 1024;
//   // collect data for 50 mS and then plot data
//   while (millis() - startMillis < sampleWindow)
//   {
//     sample = analogRead(AMP_PIN);
//     if (sample < 1024)  // toss out spurious readings
//     {
//       if (sample > signalMax)
//       {
//         signalMax = sample;  // save just the max levels
//       }
//       else if (sample < signalMin)
//       {
//         signalMin = sample;  // save just the min levels
//       }
//     }
//   }
//   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
//   Serial.println(peakToPeak);
//   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
//   Serial.println(volts);
// }


// void setupMic() {
//     i2s_config_t i2s_config = {
//         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//         .sample_rate = 44100,
//         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//         .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//         .intr_alloc_flags = 0,
//         .dma_buf_count = 8,
//         .dma_buf_len = 64,
//         .use_apll = false
//     };
//     i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
// }

// BluetoothSerial SerialBT;
// //define motor driver pins
#define IN1 14// motor 1 fwd pin
#define IN2 12 //motor 1 bck pin
#define IN3 13 //motor 2 fwd pin
#define IN4 15 //motor 2 bck pin
#define ENA 27 //motor 1 speed control
#define ENB 4 //motor 2 speed control

//define ultrasonic dist sensor pins
#define TRIG 5
#define ECHO 16

//distance queue size
#define DISTQ 10
#define STOPDIST 20

#define LED 33

void setup() {
  //SerialBT.begin("ESP32_Audio"); // Bluetooth device name
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  // analogWrite(ENA, 255);//sets speed to 255 (0-255 if using PWM)
  // analogWrite(ENB, 255);
}
//ENB and ENA motor speed can be set via analogWrite


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
void motor1Fwd(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
void motor1Bck(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}
void motor2Fwd(){
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void motor2Bck(){
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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

//turns left 90
void turnLeft(){
  motor1Fwd();
  motor2Bck();
  delay(350);
  stopCar();
}

//turns right 90
void turnRight(){
  motor1Bck();
  motor2Fwd();
  delay(330);
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


int distIndex = 0;
void loop(){
  //code
  if(distIndex == 0){
    setDistArray();
  }
  distances[distIndex%DISTQ] = measureDistance();
  distIndex++;
  //if at any time, distance is too close, car will stop
  if(getMedianDist() <= 20){
    digitalWrite(LED, HIGH);
    Serial.println("stopping car, turning around");
    stopCar();
    delay(1000);
    moveBackward();
    delay(500);
    turn180();
    Serial.println("measuring in front");
    setDistArray();
    digitalWrite(LED, LOW);
  }
  Serial.println(getMedianDist());
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