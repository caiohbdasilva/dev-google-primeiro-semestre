#include <Arduino.h>

String frase = "sEnAi eSp32 ArDuInO";

void setup() {
  Serial.begin(9600);
}

void loop() {
 String fraseM = frase;
 String frasem = frase;
 

 fraseM.toUpperCase();
 frasem.toLowerCase();
 char inicial = fraseM.charAt(0); 
 String resto_inicial = frasem.substring(1,19);
 Serial.println("------------------------------------------------------------------------------------");
 Serial.println(fraseM);
 Serial.println("------------------------------------------------------------------------------------");
 Serial.println(frasem);
 Serial.println("------------------------------------------------------------------------------------");
 Serial.print(inicial);
 Serial.println(resto_inicial);
 Serial.println("------------------------------------------------------------------------------------");
 delay(10000);
 }