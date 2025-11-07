#include <Arduino.h>
#include <Bounce2.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Bounce botaoMais = Bounce();
Bounce botaoMenos = Bounce();
LiquidCrystal_I2C lcd(0x27, 20, 4);

int nivel = 0;
int nivelAntes = 0;

void setup() {
  Serial.begin(9600);
  botaoMais.attach(13, INPUT_PULLUP);
  botaoMenos.attach(12, INPUT_PULLUP);
  pinMode(27, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("nivel: 0%");
}

void loop() {

  botaoMais.update();
  botaoMenos.update();

  if (botaoMais.fell()) nivel += 10;
  if (botaoMenos.fell()) nivel -= 10;

  if (nivel <= 0) nivel = 0;
  else if (nivel >= 100) nivel = 100;

  if (nivel != nivelAntes) {
    analogWrite(27, nivel);
    if (nivelAntes >= 100 && nivel < 100) {
      lcd.setCursor(9, 0);
      lcd.print("  ");
    }
    lcd.setCursor(7, 0);
    lcd.print(nivel);
    lcd.print("% ");
  }

  nivelAntes = nivel;
  
}