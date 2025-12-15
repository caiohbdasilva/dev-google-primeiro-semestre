#include <Arduino.h>


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.setTimeout(5000);  
  String mensagem = "Temperatura = 27C";
  String mensagemS = mensagem.substring(14, 16);
  float celsius = mensagemS.toFloat();
  float fahrenheit = (celsius*1.8)+32;
  Serial.println(fahrenheit);
}

