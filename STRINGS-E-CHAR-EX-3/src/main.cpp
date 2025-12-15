#include <Arduino.h>

char frase[] = "ESTAMOS APRENDENDO ARDUINO NA AULA";

void setup() {
    Serial.begin(9600);
}

void loop() {
if (strstr(frase, "ARDUINO"))
  {
     Serial.println("CONTEÚDO RELACIONADO A ARDUINO DETECTADO");
   } else {
    Serial.println("NÃO ENCONTRADO");
  }
  delay(5000);
}
