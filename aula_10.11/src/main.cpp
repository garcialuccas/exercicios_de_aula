#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

BluetoothSerial bt;
LiquidCrystal_I2C lcd(0x27, 20, 4);

float celsiusAntes, fahrennheitAntes, umidadeAntes;
String mensagemRecebida;

void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("Celsius: 0.0C");
  lcd.setCursor(0, 1);
  lcd.print("Fahrenheit: 0.0F");
  lcd.setCursor(0, 2);
  lcd.print("Umidade: 0.0%");

  if (bt.begin("espSlave Luccas")) {
    Serial.println("Endereço: " + bt.getBtAddressString());
    // endereço bluetooth: 14:33:5C:52:2E:36
  }
  else Serial.println("Erro ao iniciar o bluetooth");

}

void loop() {

  mensagemRecebida = bt.readStringUntil('\n');
  mensagemRecebida.trim();
  
  if (mensagemRecebida.equals("")) return;

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, mensagemRecebida);

  if (error) {
    Serial.print("Erro ao ler dados: ");
    Serial.println(error.c_str());
    return;
  }

  float celsius = doc["celsius"];
  float fahrenheit = doc["fahrenheit"];
  float umidade = doc["umidade"];

  if (celsius != celsiusAntes) {
    lcd.setCursor(9, 0);
    lcd.print("      ");
    lcd.setCursor(9, 0);
    lcd.printf("%.1fC", celsius);
  }

  if (fahrenheit != fahrennheitAntes) {
    lcd.setCursor(12, 1);
    lcd.print("      ");
    lcd.setCursor(12, 1);
    lcd.printf("%1.fF", fahrenheit);
  }

  if (umidade != umidadeAntes) {
    lcd.setCursor(9, 2);
    lcd.print("      ");
    lcd.setCursor(9, 2);
    lcd.printf("%1.f%", umidade);
  }

  celsiusAntes = celsius;
  fahrennheitAntes = fahrenheit;
  umidadeAntes = umidade;

}