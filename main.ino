// #include <Arduino.h>
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
// #define IN1 27 // motor 1 fwd pin
// #define IN2 26 //motor 1 bck pin
// #define IN3 4 //motor 2 fwd pin
// #define IN4 5 //motor 2 bck pin
// #define ENA 14 //motor 1 speed control
// #define ENB 7 //motor 2 speed control
// //all example numbers, change when we connect actual pins for L298N

// void setup() {
//   SerialBT.begin("ESP32_Audio"); // Bluetooth device name
//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
//   pinMode(IN3, OUTPUT);
//   pinMode(IN4, OUTPUT);
//   pinMode(ENA, OUTPUT);
//   pinMode(ENB, OUTPUT);

//   analogWrite(ENA, 255);//sets speed to 255 (0-255 if using PWM)
//   analogWrite(ENB, 255);

//   Serial.begin(115200);
// }
// //ENB and ENA motor speed can be set via analogWrite

// //basic motor movement methods

// //sets motor1 direction to forward
// void motor1Fwd(){
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
// }
// void motor1Bck(){
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
// }
// void motor1Stop(){
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
// }
// void motor2Fwd(){
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
// }
// void motor2Bck(){
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
// }
// void motor2Stop(){
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
// }

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