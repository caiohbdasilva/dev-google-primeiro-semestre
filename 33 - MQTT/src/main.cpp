#include <Arduino.h>
#include "internet.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_client_id = "ESP32_caioba";
const char *mqtt_topic_sub = "senai134/devgoogleCaio/sub";
const char *mqtt_topic_pub = "senai134/devgoogleLucas/sub";

WiFiClient espClient;
PubSubClient client(espClient);

//PROTOTIPO DAS FUNÇÕES
void conectaMqtt();
void retornoMqtt(char *, byte *, unsigned int);


void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(10000);

  conectaWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(retornoMqtt);
}

void loop()
{
  checkWiFi();

  client.loop(); // Atualiza o cliente

  if (!client.connected()) // Verifica se não esta conectado
  {
    conectaMqtt(); // Executa a função de conectar
  }

  // ---------------- LEITURA DA SERIAL ----------------
  if (Serial.available() > 0)
  {
    String textoDigitado = Serial.readStringUntil('\n');
    textoDigitado.trim();

    // ---------------- NAO ENVIA TEXTO VAZIO ----------------
    if (textoDigitado.length() == 0)
    {
      Serial.println("Mensagem vazia");
      return;
    }
    // ---------------- ESTRUTURA O JSON ----------------
    JsonDocument doc;
    doc["disp"] = "ESP32_caioba";
    doc["msg"] = textoDigitado;
    doc["time"] = millis();

    String stringJson;
    serializeJson(doc, stringJson);

    // ---------------- PUBLICA NO MQTT ----------------
    client.publish(mqtt_topic_pub, stringJson.c_str());
    Serial.print("Enviado: ");
    Serial.println(stringJson);
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

  // ---------------- MONTA A STRING COM A MENSAGEM RECEBIDA ----------------
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++)
  {
    mensagemRecebida += (char)payload[i];
  }

  Serial.print("JSON recebido: ");
  Serial.println(mensagemRecebida);

  // ---------------- INTERPRETA JSON RECEBIDO ----------------
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagemRecebida);

  if (erro)
  {
    Serial.print("Erro ao decodificar JSON: ");
    Serial.print(erro.c_str());
    return;
  }

  // ---------------- ACESSA OS CAMPOS JSON ----------------
  const char *disp = doc["disp"];
  const char *msg = doc["msg"];
  unsigned long time = doc["time"]; 

  Serial.println("Dados recebidos");
  Serial.printf("Dispositivo: %s\n", disp);
  Serial.printf("Mensagem: %s\n", msg);
  Serial.printf("Tempo: %i\n", time);
  Serial.println("-------------------------");
}
