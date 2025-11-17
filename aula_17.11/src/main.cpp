#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <ezTime.h>
#include <Bounce2.h>

const char *SSID = "SENAI IoT";
const char *SENHA = "Senai@IoT";

const unsigned long tempoEsperaConexao = 5000;
const unsigned long tempoEsperaReconexao = 5000;
unsigned long tempo = 0;

String minuto, segundo, hora, dia, diaSemana, mes, ano, minutoAnteiror, segundoAnterior, horaAnterior, diaAnterior, diaSemanaAnterior, mesAnterior, anoAnterior, data, dataAnterior;
String alarme;
String horaAlarme = "";
String minutoAlarme = "";
bool statusAlarme = false;
bool pisca;

Timezone sp;

LiquidCrystal_I2C lcd(0x27, 20, 4);

Bounce botao;

void conectarWifi();
void reconectarWifi();
void atualizarTempo(String tempo, String tempoAnterior, int x, int y);
void atualizarData(String tempo, String tempoAnterior, int x, int y);
void alarmeAtivado(bool ativado);
void atualizarAlarme(String h, String m);
String formatarData(String d, String m, String y);

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(10000);

  botao.attach(23, INPUT_PULLUP);

  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);

  conectarWifi();

  waitForSync();

  sp.setLocation("America/Sao_Paulo");

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 3);
  lcd.print("alarme");

}

void loop() {

  botao.update();

  lcd.setCursor(0, 3);
  lcd.print("   alarme ");

  reconectarWifi();

  segundo = sp.dateTime("s");
  minuto = sp.dateTime("i");
  hora = sp.dateTime("H");

  dia = sp.dateTime("d");
  diaSemana = sp.dateTime("w");
  mes = sp.dateTime("m");
  ano = sp.dateTime("y");

  atualizarTempo(segundo, segundoAnterior, 12, 0);
  atualizarTempo(minuto, minutoAnteiror, 9, 0);
  atualizarTempo(hora, horaAnterior, 6, 0);

  atualizarTempo(dia, diaAnterior, 15, 1);
  atualizarTempo(mes, mesAnterior, 18, 1);
  atualizarTempo(ano, anoAnterior, 21, 1);

  data = formatarData(diaSemana, mes, ano);
  atualizarData(data, dataAnterior, 0, 1);

  if (Serial.available()) {
    alarme = Serial.readStringUntil('\n');
    alarme.trim();
    if (!alarme.equals("")) {
      horaAlarme = alarme.substring(0, 2);
      minutoAlarme = alarme.substring(3);
      atualizarAlarme(horaAlarme, minutoAlarme);
    }
  }

  if (hora.equals(horaAlarme) && minuto.equals(minutoAlarme)) statusAlarme = true;
  if (botao.fell()) {
    statusAlarme = false;
    horaAlarme = "";
    minutoAlarme = "";
  }

  if (statusAlarme) {
    if (millis() - tempo >= 500) {
      pisca = !pisca;
      tempo = millis();
    }

    if (pisca) {
      lcd.noBacklight();
      digitalWrite(15, 1);
    }
    else if (!pisca) {
      lcd.backlight();
      digitalWrite(15, 0);
    }
  }

  else {
    lcd.backlight();
    digitalWrite(15, 0);
  }

  segundoAnterior = segundo;
  minutoAnteiror = minuto;
  horaAnterior = hora;

  diaAnterior = dia;
  diaSemanaAnterior = diaSemana;
  mesAnterior = mes;
  anoAnterior = ano;

  dataAnterior = data;

}

void conectarWifi() {

  Serial.printf("\nConectando-se ao WiFi: %s\n", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWifi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWifi <= tempoEsperaConexao) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConcentado a %s\n", SSID);
    Serial.print("Endereço de IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(2, 1);
  }
  else {
    Serial.println("\nErro ao conectar ao WiFi");
  }
}

void reconectarWifi() {

  unsigned long tempoUltimaConcexao = 0;

  if (WiFi.status() != WL_CONNECTED && millis() - tempoUltimaConcexao >= tempoEsperaConexao) {
    Serial.printf("Conectando-se ao WiFi: %s", SSID);
    conectarWifi();
    tempoUltimaConcexao = millis();
  }
}

void atualizarTempo(String tempo, String tempoAnterior, int x, int y) {

  if (!tempo.equals(tempoAnterior)) {
    lcd.setCursor(x, y);
    lcd.print(tempo);
  }

}

void atualizarData(String tempo, String tempoAnterior, int x, int y) {

  if (!tempo.equals(tempoAnterior)) {
    lcd.setCursor(2, 1);
    lcd.print("                           ");
    lcd.setCursor(2, 1);
    lcd.print(tempo);
  }
}

void atualizarAlarme(String h, String m) {

  lcd.setCursor(10, 3);
  lcd.print(h + ":");
  lcd.setCursor(13, 3);
  lcd.print(m);

}

String formatarData(String d, String m, String y) {

  String dias[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
  String meses[] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};

  m = meses[m.toInt() - 1];
  d = dias[d.toInt() - 1];

  String dataf = d + " de " + m + " de " + y;

  return dataf;

}