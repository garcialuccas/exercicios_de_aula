#include <Arduino.h>
#include <Servo.h>
// #include <ArduinoJson.h>
// #include <BluetoothSerial.h>
// #include <cmath>

// BluetoothSerial bt;
// JsonDocument doc;
Servo servo;
float gasAnterior;

#define pinServo 33

void porta(bool abrir) {

  if (abrir) {
    for (int i = 1; i < 179; i += 2) {
      servo.write(i);
      delay(15);
    }
  }

  else if (!abrir) {
    for (int i = 179; i > 1; i -= 2) {
      servo.write(i);
      delay(15);
    }
  }

}

void setup() {

  // Serial.begin(9600);

  // if (bt.begin("espSlave Luccas")) {
  //   Serial.println("Endereço: " + bt.getBtAddressString());
  // }
  // else Serial.println("Erro ao iniciar o bluetooth");

  servo.attach(pinServo);
  servo.write(90);

}

void loop() {

  // String mensagemRecebida = bt.readStringUntil('\n');
  // mensagemRecebida.trim();

  // DeserializationError error = deserializeJson(doc, mensagemRecebida);

  // if (error) {
  //   Serial.println("Erro ao ler Json");
  //   Serial.println(error.c_str());
  //   return;
  // }

  // float gas = doc["gas"];
  
  // if (300 >= abs(gas - gasAnterior)) {
  //   if (gas >= 2500) {
  //     porta(true);
  //     Serial.println("abrir servo");
  //   }
  //   else {
  //     porta(false);
  //     Serial.println("fechar servo");
  //   }
  // }

  // gasAnterior = gas;
  
  porta(true);
  delay(2000); 
  porta(false);
  delay(2000); 
}