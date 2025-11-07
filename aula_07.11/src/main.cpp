// bluetooth slave
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

String lerBluetooth(String recebida) {
  
  String leitura = "";
  if (recebida.startsWith("celsius")) {
    leitura = recebida.substring(8);
  }
  else if (recebida.startsWith("fahrenheit")) {
    leitura = recebida.substring(11);
  }
  else if (recebida.startsWith("umidade")) {
    leitura = recebida.substring(9);
  }

  return leitura;
}

void escreverLcd(String recebida, String leitura, LiquidCrystal_I2C tela) {

  if (recebida.startsWith("celsius")) {
    tela.setCursor(0, 0);
    tela.printf("Celsius: %sC", leitura);
  }

  else if (recebida.startsWith("fahrenheit")) {
    tela.setCursor(0, 1);
    tela.printf("Fahrenheit: %sF", leitura);
  }

  else if (recebida.startsWith("umidade")) {
    tela.setCursor(0, 2);
    tela.printf("Umidade: %s%", leitura);
  }
}

LiquidCrystal_I2C lcd(0x27, 20, 4);
Bounce botao = Bounce();
BluetoothSerial bt;
String mensagemRecebida; 
String celsius = "0.0";
String celsiusAnterior = "0.0";
String fahrenheit = "0.0";
String fahrenheitAnterior = "0.0";
String umidade = "0";
String umidadeAnterior = "0";

void setup() {
  Serial.begin(115200);

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

  lcd.init();
  lcd.backlight();
  lcd.print("Celsius: 0.0C");
  lcd.setCursor(0, 1);
  lcd.print("Fahrenheit: 0.0F");
  lcd.setCursor(0, 2);
  lcd.print("Umidade: 0%");

}

void loop() {

  mensagemRecebida = bt.readStringUntil('\n');
  mensagemRecebida.trim();
  mensagemRecebida.toLowerCase();

  if(!mensagemRecebida.equals("")) {
    Serial.println(mensagemRecebida);
    celsius = lerBluetooth(mensagemRecebida);
    fahrenheit = lerBluetooth(mensagemRecebida);
    umidade = lerBluetooth(mensagemRecebida);
    
    if (celsius != celsiusAnterior && !celsius.equals("")) escreverLcd(mensagemRecebida, celsius, lcd);
    if (fahrenheit != fahrenheitAnterior && !fahrenheit.equals("")) escreverLcd(mensagemRecebida, fahrenheit, lcd);
    if (umidade != umidadeAnterior && !umidade.equals("")) escreverLcd(mensagemRecebida, umidade, lcd);
  
  }

  celsiusAnterior = celsius;
  umidadeAnterior = umidade;
  fahrenheitAnterior = fahrenheit;

}