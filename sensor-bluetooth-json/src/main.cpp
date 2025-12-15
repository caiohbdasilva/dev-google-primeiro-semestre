
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <DHT_U.h>
#include <BluetoothSerial.h>

#define DHTPIN 5 
#define DHTTYPE    DHT22 

DHT dht(DHTPIN, DHTTYPE);


float tmpC;
float tmpAntC = 0;
float tmpF;
float tmpAntF = 0;
float umd;
float umdAnt = 0;

BluetoothSerial BT;
//F4:65:0B:54:70:12
uint8_t endSlave[] = {0xF4, 0x65, 0x0B, 0x54, 0x70, 0x12};


void setup()
{
  Serial.begin(9600);

  //DHT
  dht.begin();
  

  //Bluetooth
    Serial.begin(9600);
    Serial.setTimeout(5000);

    if (BT.begin("EspMasterCaio", true))//master
    {
        Serial.println("Bluetooth iniciado com sucesso");
    }else
    {
        Serial.println("Erro ao iniciar o bluetooth");
    }

    if (BT.connect(endSlave))
    {
        Serial.println("Conectado com sucesso");
    }else{
        Serial.println("Erro ao conectar");
    } 

}
void loop()
{
  float umd = dht.readHumidity();
  float tmpC = dht.readTemperature();
  float tmpF = dht.readTemperature(true);


  JsonDocument doc;
  doc["tmpC"] = tmpC;
  doc["tmpF"] = tmpF;
  doc["umd"] = umd;



  String jsonString;
  serializeJson(doc, jsonString);
/*
  Serial.println("JSON gerado: ");
  Serial.println(jsonString);
*/
/*
  //Envio Bluetooth
  if (BT.available())''
    if (Serial.available())
    {
        String mensagemEnviar = jsonString;
        mensagemEnviar.trim();
        BT.println(jsonString);
    }
*/
    if (tmpC != tmpAntC){
      tmpAntC = tmpC;
      //if (BT.available()){
        //if (Serial.available())
          //{
            //String mensagemEnviar = jsonString;
            BT.println(jsonString);
          //}
    }
    if (tmpF != tmpAntF){
      tmpAntF = tmpF;
     // if (BT.available()){
       // if (Serial.available())
         // {
         //   String mensagemEnviar = jsonString;
            BT.println(jsonString);
         // }
    }
    //}
    if (umd != umdAnt){
      umdAnt = umd;
     // if (BT.available()){
       // if (Serial.available())
        //  {
        //    String mensagemEnviar = jsonString;
            BT.println(jsonString);
          //}
    }
    }
    
//}
//}
