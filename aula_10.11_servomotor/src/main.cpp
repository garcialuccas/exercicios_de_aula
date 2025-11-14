#include <Arduino.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <cmath>

BluetoothSerial bt;
JsonDocument doc;
Servo servo;
float gasAnterior;
bool aberto = false;

#define pinServo 32

void porta(bool abrir) {

  if (abrir) {
    for (int i = 1; i < 179; i += 2) {
      servo.write(i);
    }
  }

  else if (!abrir) {
    for (int i = 179; i > 1; i -= 2) {
      servo.write(i);
    }
  }

}

void setup() {

  Serial.begin(9600);

  pinMode(12, OUTPUT);

  if (bt.begin("espSlave Luccas")) {
    Serial.println("Endereço: " + bt.getBtAddressString());
  }
  else Serial.println("Erro ao iniciar o bluetooth");

  servo.attach(pinServo);
  servo.write(90);

}

void loop() {

  String mensagemRecebida = bt.readStringUntil('\n');
  mensagemRecebida.trim();

  if (mensagemRecebida.equals("")) return;

  DeserializationError error = deserializeJson(doc, mensagemRecebida);

  if (error) {
    Serial.println("Erro ao ler Json");
    Serial.println(error.c_str());
    return;
  }

  float gas = doc["gas"];
  
  if (300 >= abs(gas - gasAnterior)) {
    if (gas >= 2500 && !aberto) {
      porta(true);
      Serial.println("abrir servo");
      aberto = true;
      digitalWrite(12, 1);
    }
    else if (gas < 2500 && aberto){
      porta(false);
      Serial.println("fechar servo");
      aberto = false;
      digitalWrite(12, 0);
    }
  }

  gasAnterior = gas;
}