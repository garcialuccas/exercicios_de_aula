#include <Arduino.h>

String comando;

// // setup para os exericios 1, 2 e 4
void setup() {
  pinMode(23, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(5000);
}

// // exercicio 1
// void loop() {

// Serial.println("Digite um comando: ");
// comando = Serial.readStringUntil('\n');
// comando.trim();
// Serial.println(comando);

//   if (comando.endsWith("ON")) {
//     digitalWrite(23, 1);
//     Serial.println("Led Ligado");
//   }

//   else if (comando.endsWith("OFF")) {
//     digitalWrite(23, 0);
//     Serial.println("Led Desligado");
//   }

//   else if (comando.equals("")) {}

//   else {
//     Serial.println("Comando inválido");
//   }

//   comando = "";

// }

// // exericio 2
// void loop() {

//   Serial.println("Digite um comando: ");
//   comando = Serial.readStringUntil('\n');
//   comando.trim();
//   Serial.println(comando);

//   if (!comando.equals("")) {

//     String numero = "";

//     for (int i = 0; i < comando.length(); i++) {
//       if (isdigit(comando.charAt(i))) {
//         numero.concat(comando.charAt(i));
//       }
//     }

//     float celsius = numero.toFloat();
//     float fahrenheit = (celsius * 1.8) + 32;

//     Serial.printf("Fahrenheit: %f°F\n", fahrenheit);

//   }

// }

// // exercicio 3
// void setup() {
  
//   Serial.begin(9600);

//   char frase[] = "ESTAMOS APRENDENDO ARDUINO NA AULA";

//   if (strstr(frase, "ARDUINO")) Serial.println("CONTEÚDO RELACIONADO A ARDUINO DETECTADO");
//   else Serial.println("NÃO ENCONTRADO");

// }

// void loop() {}

// exericio 4
void loop() {

  Serial.println("Digite um comando: ");
  comando = Serial.readStringUntil('\n');
  comando.trim();
  Serial.println(comando + "\n");

  if (!comando.equals("")) {
    comando.toUpperCase();
    Serial.println(comando + "\n");

    comando.toLowerCase();
    Serial.println(comando + "\n");
    
    comando.setCharAt(0, toupper(comando.charAt(0)));
    Serial.println(comando);
  }

}