#include <Arduino.h>
#include "internet.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal_I2C.h>

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_Grasi";    
const char *mqtt_topic_sub2 = "senai134/devgoogle/pubgrupo3distancia";
bool BotaoA;
bool BotaoB;
bool BotaoC;
bool BotaoD;
bool BotaoE;
bool BotaoF;
int velocidade;
int distancia;
LiquidCrystal_I2C lcd(0x27,20,4);
WiFiClient espClient;
PubSubClient client(espClient);

//PROTOTIPO DAS FUNÇÕES
void conectaMqtt();
void retornoMqtt(char *, byte *, unsigned int);


void setup()
{
    lcd.init();

    lcd.backlight();

  Serial.begin(115200);
  
  Serial.setTimeout(10000);

  conectaWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(retornoMqtt);

    lcd.setCursor(0,0);
    lcd.print("St Motor: ");
    lcd.setCursor(0,1);
    lcd.print("Potencia: ");
    lcd.setCursor(0,2);
    lcd.print("Distancia: ");

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
  if (BotaoA == 1)
  {
    lcd.setCursor(10,0);
    lcd.print("Frente   ");
  }

  if (BotaoB == 1)
  {
    lcd.setCursor(10,0);
    lcd.print("Direita   ");
  }

  if (BotaoC == 1)
  {
    lcd.setCursor(10,0);
    lcd.print("Tras   ");
  }

  if (BotaoD == 1)
  {
    lcd.setCursor(10,0);
    lcd.print("Esquerda  ");
  }

  if(BotaoA == 0 && BotaoB == 0 && BotaoC == 0 && BotaoD == 0){
    lcd.setCursor(10,0);
    lcd.print("Parado   ");
  }


    
    lcd.setCursor(10,1);
    lcd.print(velocidade);
    lcd.print("     ");

    
    
    lcd.setCursor(10,2);
    lcd.print(distancia);
    lcd.print("      ");

    
}

void conectaMqtt()
{
  while (!client.connected())
  {
    Serial.println("Conectando ao Mqtt...");
    if (client.connect(mqtt_client_id))
    {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic_sub2);
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
  BotaoA = doc["frente"];
  BotaoB = doc["direita"];
  BotaoC = doc["tras"];
  BotaoD = doc["esquerda"];
  velocidade = doc["velocidade"];
  distancia = doc["distancia"];


  Serial.println("Dados recebidos");
  Serial.print("Botao A: ");
  Serial.println(BotaoA);
  Serial.print("Botao B: ");
  Serial.println(BotaoB);
  Serial.print("Botao C: ");
  Serial.println(BotaoC);
  Serial.print("Botao D: ");
  Serial.println(BotaoD);
  Serial.print("Velocidade: ");
  Serial.println(velocidade);
  Serial.print("Distancia MM: ");
  Serial.println(distancia);
  Serial.println("-------------------------");
}



