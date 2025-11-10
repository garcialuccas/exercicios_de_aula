#include <Arduino.h>
#include <HardwareSerial.h>

#define pinRX    16
#define pinTX    17

void setup() {
   Serial.begin(9600);
   Serial1.begin(9600, SERIAL_8N1, pinRX, pinTX);

   Serial.println("Transmissor Lora V1.0 Nov/2025");
}

void loop() {        
  Serial.println(Serial1.readStringUntil('\n'));
  Serial1.print("olá, bom dia!\n");
  delay(250);  
}