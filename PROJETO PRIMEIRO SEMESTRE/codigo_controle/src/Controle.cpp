#include <Arduino.h>
#include "internet.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Bounce2.h>

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_Gus";
const char *mqtt_topic_sub = "senai134/devgoogle/subgrupo3";
const char *mqtt_topic_pub = "senai134/devgoogle/pubgrupo3";
const char *disp;
const char *msg;
unsigned long tempo;
unsigned long tempoAnterior = 0;
unsigned long tempoAtual;
int velocidade;


Bounce Frente = Bounce();
Bounce Direita = Bounce();
Bounce Tras = Bounce();
Bounce Esquerda = Bounce();
Bounce BotaoE = Bounce();
Bounce BotaoF = Bounce();
Bounce BotaoK = Bounce();

WiFiClient espClient;
PubSubClient client(espClient);


void conectaMqtt();
void retornoMqtt(char *, byte *, unsigned int);


void setup()
{
  Frente.attach(12, INPUT_PULLUP);

  Direita.attach(14, INPUT_PULLUP);

  Tras.attach(26, INPUT);

  Esquerda.attach(27, INPUT);

  BotaoE.attach(19, INPUT);

  BotaoF.attach(18, INPUT);

  BotaoK.attach(21, INPUT);

  Serial.begin(115200);
 
  Serial.setTimeout(10000);

  conectaWiFi();  

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(retornoMqtt);
}

void loop()
{

  tempoAtual = millis();
  int debouncedStateA = Frente.read();
  int debouncedStateB = Direita.read();
  int debouncedStateC = Tras.read();
  int debouncedStateD = Esquerda.read();
  BotaoE.read();
  BotaoF.read();
  BotaoK.read();


  Frente.update();

  Direita.update();

  Tras.update();
  
  Esquerda.update();

  BotaoE.update();

  BotaoF.update();

  BotaoK.update();
  
  checkWiFi();

  client.loop(); 

  if (!client.connected()) 
  {
    conectaMqtt(); 
  }

  
  if (Frente.changed() || Direita.changed() || Tras.changed() ||Esquerda.changed() || BotaoE.changed() || BotaoF.changed() || BotaoK.changed())
  {

    JsonDocument doc;
    doc["BotaoA"] = !Frente.read();
    doc["BotaoB"] = !Direita.read(); 
    doc["BotaoC"] = !Tras.read();
    doc["BotaoD"] = !Esquerda.read();
    doc["BotaoE"] = !BotaoE.read();
    doc["BotaoF"] = !BotaoF.read();
    doc["BotaoK"] = !BotaoK.read();
    doc["Velocidade"] = velocidade;

    String stringJson;
    serializeJson(doc, stringJson);

    
    client.publish(mqtt_topic_pub, stringJson.c_str());
    Serial.print("Enviado: ");
    Serial.println(stringJson);

    tempoAnterior = tempoAtual;
  }

  if (BotaoE.fell())
  {
    velocidade +=10;
    if(velocidade >= 100){
      velocidade = 100;
    }
  }

  if (BotaoF.fell())
  {
    velocidade -= 10;
    if(velocidade <= 0){
      velocidade = 0;
    }
  }
  
}

void conectaMqtt()
{
  while (!client.connected())
  {
    Serial.println("Conectando ao Mqtt...");
    if (client.connect(mqtt_client_id))
    {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic_sub);
    }
    else
    {
      Serial.print("Falha :");
      Serial.print(client.state());
      Serial.print("Tentando novamente em 5s.");
      delay(5000);
    }
  }
}

void retornoMqtt(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Mensagem recebida em: ");
  Serial.print(topic);
  Serial.print(": ");

  
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++)
  {
    mensagemRecebida += (char)payload[i];
  }

  Serial.print("JSON recebido: ");
  Serial.println(mensagemRecebida);

 
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagemRecebida);

  if (erro)
  {
    Serial.print("Erro ao decodificar JSON: ");
    Serial.print(erro.c_str());
    return;
  }

  
  disp = doc["disp"];
  msg = doc["msg"];
  tempo = doc["time"]; 

  Serial.println("Dados recebidos");
  Serial.print("Dispositivo: ");
  Serial.println(disp);
  Serial.print("Mensagem: ");
  Serial.println(msg);
  Serial.print("tempo: ");
  Serial.println(tempo);
  Serial.println("-------------------------");
}