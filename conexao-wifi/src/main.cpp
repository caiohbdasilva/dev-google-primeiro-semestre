#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

//Prototipar as funcoes
void conexaowifi();
void reconexaowifi();

const char *SSID = "SALA 25";
const char *SENHA = "info@134";

const unsigned long tempoEsperaConexao = 5000;
const unsigned long tempoEsperaReconexao = 5000;

Timezone sp;



void setup() {
  
  Serial.begin(9600);
  
  conexaowifi();  

  waitForSync();

  sp.setLocation("America/Sao_Paulo");
  // sp.setPosix("UTC-3"); // mesma coisa da de cima

  Serial.println(sp.dateTime());
  Serial.println(sp.dateTime("d/m/Y H:i:s"));

  Serial.println(sp.now());

}

void loop() {
  reconexaowifi();
}

void conexaowifi(){
  Serial.printf("Conectando-se ao wi-fi: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWiFi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWiFi < tempoEsperaConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Wifi conectado com sucesso!");
    Serial.print("Enndereço de IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha ao se conectar");
  }
}

void reconexaowifi(){
  unsigned long tempoAtual = millis();
  unsigned long tempoUltimaConexao = 0;

  if (tempoAtual - tempoUltimaConexao > tempoEsperaReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexão WiFi perdida");
      conexaowifi();
    }
    

    tempoUltimaConexao = tempoAtual;
  }
  
}
