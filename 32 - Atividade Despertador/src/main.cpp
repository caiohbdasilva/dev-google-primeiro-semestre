#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"

// CONFIGURAÇÕES -----------------------------------------
const char* SSID = "SALA 25";
const char* SENHA = "info@134";

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinoLed = 25;
const int pinoBotao = 14;

bool alarmeAtivo = false;
bool alarmeDisparado = false;

int horaAlarme = 0, minutoAlarme = 0;

unsigned long ultimoUpdate = 0;

// FUNÇÃO PARA AJUSTE DE HORA -----------------------------------------
void sincronizarHorario() {
  configTime(-3 * 3600, 0, "pool.ntp.org");
  Serial.println("Sincronizando horário...");
  delay(2000);
}

// CONFIGURAÇÃO INICIAL -----------------------------------------------
void setup() {
  Serial.begin(9600);

  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLDOWN);

  lcd.init();
  lcd.backlight();

  WiFi.begin(SSID, SENHA);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConectado!");

  sincronizarHorario();

  lcd.setCursor(0, 0);
  lcd.print("Defina alarme:");
  lcd.setCursor(0, 1);
  lcd.print("HH MM");

  // Aguarda usuário definir horário via Serial
  Serial.println("\nDigite o horário do alarme (ex: 06 30): ");
  while (Serial.available() == 0) {}
  horaAlarme = Serial.parseInt();
  minutoAlarme = Serial.parseInt();

  alarmeAtivo = true;

  Serial.print("Alarme definido: ");
  Serial.print(horaAlarme);
  Serial.print(":");
  Serial.println(minutoAlarme);
}

// FUNÇÃO PARA OBTER DATA/HORA ----------------------------------------
void obterHorario(int &h, int &m, int &s, int &d, int &mes, int &ano) {
  struct tm tempo;
  if (!getLocalTime(&tempo)) return;

  h = tempo.tm_hour;
  m = tempo.tm_min;
  s = tempo.tm_sec;
  d = tempo.tm_mday;
  mes = tempo.tm_mon + 1;
  ano = tempo.tm_year + 1900;
}

// LOOP PRINCIPAL ------------------------------------------------------
void loop() {
  int hora, minuto, segundo, dia, mes, ano;
  obterHorario(hora, minuto, segundo, dia, mes, ano);

  // --- TESTE DO BOTÃO ---
  int estadoBotao = digitalRead(pinoBotao);
  Serial.print("Botão: ");
  Serial.println(estadoBotao);

  // --- ACIONAR ALARME ---
  if (alarmeAtivo && !alarmeDisparado && hora == horaAlarme && minuto == minutoAlarme && segundo == 0) {
    alarmeDisparado = true;
    Serial.println("ALARME DISPARADO!");
  }

  // --- PISCAR LED E LCD QUANDO DISPARADO ---
  if (alarmeDisparado) {
    digitalWrite(pinoLed, LOW);
    lcd.noBacklight();
    delay(1000);
    digitalWrite(pinoLed, HIGH);
    lcd.backlight();
    delay(1000);

    // Se botão for pressionado, desativa o alarme
    if (estadoBotao == HIGH) {
      alarmeDisparado = false;
      alarmeAtivo = false;
      digitalWrite(pinoLed, LOW);
      Serial.println("Alarme DESATIVADO pelo botão.");
    }
  }

  // --- ATUALIZAR LCD A CADA 300ms ---
  if (millis() - ultimoUpdate > 300) {
    ultimoUpdate = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(dia);
    lcd.print("/");
    lcd.print(mes);
    lcd.print("/");
    lcd.print(ano);

    lcd.setCursor(0, 1);
    lcd.print(hora);
    lcd.print(":");
    lcd.print(minuto);
    lcd.print(":");
    lcd.print(segundo);

    lcd.print(" ");

    if (alarmeAtivo)
      lcd.print("A");
    else
      lcd.print("D"); // Desativado
  }
}
