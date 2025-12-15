#include <Arduino.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);


#define LED 33
#define botao_aumenta 32 
#define botao_diminui 27 

Bounce bounceaumenta = Bounce();
Bounce bouncediminui = Bounce();

int contagem;
int ledmap;
bool estadoled;
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Nv. de Intensidade: ");

  ledmap = map(LED, 0, 876, 0, 100);

  pinMode(LED, OUTPUT);
  bounceaumenta.attach(botao_aumenta, INPUT_PULLUP);
  bouncediminui.attach(botao_diminui, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  
  bounceaumenta.update();
  bouncediminui.update();

     
  if (bounceaumenta.fell()){
    contagem = contagem+10;
    Serial.println(contagem);
    }

  if (bouncediminui.fell()){
    contagem = contagem-10;
    }

  if (estadoled){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }


  if (contagem<0){
    contagem = 0;
  }

   if (contagem>=100){
    contagem = 100;
  }
  
  analogWrite(LED, contagem);
  
  lcd.setCursor(0,1);
  lcd.printf("%i   ", contagem);

  }

