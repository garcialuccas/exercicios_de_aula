// bluetooth slave
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Bounce2.h>

Bounce botao = Bounce();
BluetoothSerial bt;
String mensagemRecebida;
String mensagemEnviada = "";
String mensagemBotaoAnterior = "a";
String mensagemBotao = "";
bool led = false;
bool piscar = false;
bool ledAmigo = false;
bool ledAmigoAnterior = true;
bool piscaAmigo = false;
unsigned long tempo = 0;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5000);

  pinMode(32, OUTPUT);
  botao.attach(14, INPUT_PULLUP);
  botao.interval(50);
  
  if (bt.begin("espSlave luccas")) {
    Serial.println("bluetooth ligado");
    Serial.print("endereço bluetooth: ");
    Serial.println(bt.getBtAddressString());
    // endereço bluetooth: 14:33:5C:52:2E:36
  }
  else Serial.println("erro ao iniciar o bluetooth");
}

void loop() {

  botao.update();

  if (botao.fell()) ledAmigo = !ledAmigo;
 
  if (ledAmigo != ledAmigoAnterior) {
    if (ledAmigo) mensagemBotao = "liga";
    else if (!ledAmigo) mensagemBotao = "desliga";
  }

  if (!botao.read() && botao.currentDuration() >= 2000) mensagemBotao = "pisca";

  if (mensagemBotao != mensagemBotaoAnterior) {
    bt.println(mensagemBotao);
    Serial.println(mensagemBotao);
  }

  mensagemBotaoAnterior = mensagemBotao;
  ledAmigoAnterior = ledAmigo;

  if (bt.available()) {
    mensagemRecebida = bt.readStringUntil('\n');
    mensagemRecebida.trim();
    if (!mensagemRecebida.equals("")) {
      Serial.printf("olha a mensagem: %s\n", mensagemRecebida);
    }
  }

  if (Serial.available()) {
    mensagemEnviada = Serial.readStringUntil('\n');
    mensagemEnviada.trim();
    if (!mensagemEnviada.equals("")) {
      Serial.printf("enviado: %s\n", mensagemEnviada);
      bt.println(mensagemEnviada);
    }
  }
  
  if (mensagemRecebida.equalsIgnoreCase("ligar")) {
    led = true;
    piscar = false;
  }

  if (mensagemRecebida.equalsIgnoreCase("desligar")) {
    led = false;
    piscar = false;
  }

  if (mensagemRecebida.equalsIgnoreCase("piscar")) {
    piscar = true;
  }

  if (piscar) {
    if (millis() - tempo >= 500) {
      led = !led;
      tempo = millis();
    }
  }

  digitalWrite(32, led);

}
