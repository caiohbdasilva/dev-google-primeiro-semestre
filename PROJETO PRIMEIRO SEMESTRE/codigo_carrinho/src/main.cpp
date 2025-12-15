
#include <Arduino.h>
#include "internet.h"
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X sensorDianteira = Adafruit_VL53L0X();


bool frente;
bool tras;
bool esquerda;
bool direita;
bool botaoE;
bool botaoF;
bool botaoK;
int velocidade;
int velocidademap;
int distancia;

const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_client_id = "senai134_esp_caioba";
const char *mqtt_topic_sub = "senai134/devgoogle/pubgrupo3";
const char *mqtt_topic_pub = "senai134/devgoogle/pubgrupo3distancia";

unsigned long tempoAnterior = 0;
unsigned long tempoAtual;

WiFiClient espClient;
PubSubClient client(espClient);

// PROTOTIPO DAS FUNÇÕES
void conectaMqtt();
void retornoMqtt(char *, byte *, unsigned int);

// ------------------------------
// Pinos do L298N
// ------------------------------

// Motor A
const int enA = 14; // PWM (canal 0)
const int in1 = 27;
const int in2 = 26;
// Motor B
const int enB = 15; // PWM (canal 1)
const int in3 = 2;
const int in4 = 4;


const int freq = 1000;    // 1 kHz
const int resolution = 8; // 8 bits (0–255)
const int pwmA = 0;       // canal 0
const int pwmB = 1;       // canal 1


// }

// ------------------------------
// SETUP
// ------------------------------
void setup()
{
  Serial.begin(9600);
  // Sensor de distância
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!sensorDianteira.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }

  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));

  //Rodas
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);



  ledcSetup(pwmA, freq, resolution);
  ledcSetup(pwmB, freq, resolution);

  

  Serial.begin(9600);
  Serial.setTimeout(10000);

  conectaWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(retornoMqtt);

  Wire.begin();
}


void loop()
{

  tempoAtual = millis();

  checkWiFi();

  client.loop(); // Atualiza o cliente
  velocidademap = map(velocidade, 0, 100, 0, 255);

  if (!client.connected()) // Verifica se não esta conectado
  {
    conectaMqtt(); // Executa a função de conectar
  }

  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  sensorDianteira.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    distancia = measure.RangeMilliMeter;
  } else {
    Serial.println(" out of range ");
  }
    
  delay(100); 

  // FRENTE
  if (frente)
  {
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);

    digitalWrite(in3, 1);
    digitalWrite(in4, 0);


    for (int i = 0; i <= 255; i++)
  {
    ledcWrite(pwmA, i);
    ledcWrite(pwmB, i);
  }
}

  // TRÁS
  else if (tras)
  {
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);

    digitalWrite(in3, 0);
    digitalWrite(in4, 1);

  
  }
  else if (direita)
  {
    digitalWrite(in1, 0);
    digitalWrite(in2, 1);

    digitalWrite(in3, 0);
    digitalWrite(in4, 1);

    ledcWrite(pwmA, velocidademap);
    ledcWrite(pwmB, velocidademap);
  }
  else if (esquerda)
  {
    digitalWrite(in1, 1);
    digitalWrite(in2, 0);

    digitalWrite(in3, 1);
    digitalWrite(in4, 0);                         

    ledcWrite(pwmA, velocidademap);
    ledcWrite(pwmB, velocidademap);
  }
   else if(!frente && !tras && !direita && !esquerda)
  {
    digitalWrite(in1, 0);
    digitalWrite(in2, 0);

    digitalWrite(in3, 0);
    digitalWrite(in4, 0);
  }

 if(measure.RangeMilliMeter <= 200)
 {
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);

  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
  }
if (tempoAtual - tempoAnterior > 250)
{
   JsonDocument doc;
    doc["disp"] = "Esp32Caioba";
    doc["distancia"] = distancia;
    doc["frente"] = frente;
    doc["tras"] = tras;
    doc["esquerda"] = esquerda;
    doc["direita"] = direita;
    doc["velocidade"] = velocidade;

    String stringJson;
    serializeJson(doc, stringJson);

    // ---------------- PUBLICA NO MQTT ----------------
    client.publish(mqtt_topic_pub, stringJson.c_str());

    tempoAtual = tempoAnterior;
}

Serial.println(velocidade);

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

  // ---------------- MONTA A STRING COM A MENSAGEM RECEBIDA ----------------
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++)
  {
    mensagemRecebida += (char)payload[i];
  }

  // ---------------- INTERPRETA JSON RECEBIDO ----------------
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagemRecebida);

  if (erro)
  {
    Serial.print("Erro ao decodificar JSON: ");
    Serial.print(erro.c_str());
    return;
  }

  frente = doc["BotaoA"];
  direita = doc["BotaoB"];
  tras = doc["BotaoC"];
  esquerda = doc["BotaoD"];
  botaoE = doc["BotaoE"];
  botaoF = doc["BotaoF"];
  botaoK = doc["BotaoK"];
  velocidade = doc["Velocidade"];

  // Serial.print("JSON recebido: ");
  // Serial.println(mensagemRecebida);
  // Serial.println(velocidademap);

}
