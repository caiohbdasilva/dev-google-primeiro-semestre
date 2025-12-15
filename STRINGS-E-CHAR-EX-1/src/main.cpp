#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Teste");
  Serial.begin(9600);
}

void loop()
{
  Serial.setTimeout(5000);  
  String texto = Serial.readStringUntil('\n');
  texto.trim();
    if (texto.endsWith("ON"))
    {
        Serial.println("LIGAR LED");
    } else {
        if (texto.endsWith("OFF"))
        {
           Serial.println("DESLIGAR LED");
        } else {
            Serial.println("COMANDO INVÁLIDO");
        }
        
     }
}

