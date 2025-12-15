#include <Arduino.h>
#include <WiFi.h>
#include <ezTime.h>

// prototipar as funcoes
void conexaoWifi();
void reconexaoWifi();

const char *SSID = "SALA 25";
const char *SENHA = "info@134";
const unsigned long tempoEsperaConexao = 10000;
const unsigned long tempoEsperaReconexao = 5000;

Timezone sp;

void setup()
{
  Serial.begin(9600);

  conexaoWifi();       
  waitForSync();       

  sp.setLocation("America/Sao_Paulo");
}

void loop()
{
  reconexaoWifi();

  if (minuteChanged())
  {
    Serial.print("Hoje é ");
    Serial.print(sp.dateTime("l, d"));
    Serial.print(" de ");
    Serial.print(sp.dateTime("F"));
    Serial.print(" de ");
    Serial.print(sp.dateTime("Y"));
    Serial.print(", agora são ");
    Serial.print(sp.dateTime("h:i:s"));
    Serial.print(" - ");
    Serial.print(sp.dateTime("A"));
    Serial.print(" (BRT)");
  }
}

void conexaoWifi()
{
  Serial.printf("Conectando ao Wifi: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoInicialWifi = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - tempoInicialWifi < tempoEsperaConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWifi conectado com sucesso!");
    Serial.print("Endereco IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFalha ao se conectar");
  }
}

void reconexaoWifi()
{
  static unsigned long tempoUltimaConexao = 0;  // **precisa ser static**
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoUltimaConexao > tempoEsperaReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexao Wifi perdida, tentando reconectar...");
      conexaoWifi();
    }
    tempoUltimaConexao = tempoAtual;
  }
}
