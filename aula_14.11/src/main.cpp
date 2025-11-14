#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

const char *SSID = "SENAI IoT";
const char *SENHA = "Senai@IoT";

const unsigned long tempoEsperaConexao = 5000;
const unsigned long tempoEsperaReconexao = 5000;

unsigned int minuto, minutoAnterior;

Timezone sp;

void conectarWiFi();
void reconectarWiFi();

void setup() {

  Serial.begin(115200);

  pinMode(2, OUTPUT);

  conectarWiFi();

  waitForSync();

  sp.setLocation("America/Sao_Paulo");

}

void loop() {

  reconectarWiFi();

  minuto = sp.minute();

  if (minuto != minutoAnterior) {
    Serial.print("\r                                                                      \r");
    Serial.print(sp.dateTime("\\Ho\\j\\e é l, d \\d\\e F \\d\\e Y, \\a\\gor\\a \\são h:i:s - A (\\B\\R\\T)"));
  }

  minutoAnterior = minuto;

}

void conectarWiFi() {
  Serial.printf("\nConectando-se ao wifi: %s\n", SSID);
  
  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWiFi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWiFi <= tempoEsperaConexao) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConectado a %s\n", SSID);
    Serial.print("Endereço de IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(2, 1);
  }
  else {
    Serial.println("\nErro ao conectar ao WiFi");
  }
}

void reconectarWiFi() {

  unsigned long tempoUltimaConexao = 0;
  
  if (WiFi.status() != WL_CONNECTED && millis() - tempoUltimaConexao >= tempoEsperaReconexao){
    Serial.printf("Conectando-se ao WiFi: %s", SSID);
    conectarWiFi();
    tempoUltimaConexao = millis();
  }
}