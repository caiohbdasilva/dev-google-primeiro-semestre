/*
#include <Arduino.h>
#include <BluetoothSerial.h>  

 BluetoothSerial BT;


 void setup() {
   Serial.begin(9600);
 Serial.setTimeout(5000);
   if (BT.begin("aperto")) {
     Serial.println("Bluetooth initialized");
   Serial.print ("Endereco BLUT:  ");
   //F4:65:0B:54:70:12
   Serial.println(BT.getBtAddressString());
  
   } else {
     Serial.println("ERROR");
   } 

 }


 void loop() {
 if (BT.available())
 {
   String mensagemRecebida = BT.readStringUntil('\n');
   mensagemRecebida.trim();
   Serial.printf("Mensagem Recebida: %s", mensagemRecebida);
 }

 if(Serial.available());
 {
   String mensagemEnviar = Serial.readStringUntil('\n');
   mensagemEnviar.trim();
   BT.println(mensagemEnviar);
 }


 }
 */

// ------------------------ BLUETOOTH MASTER -----------------------------

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Bounce2.h>
#define pinLed 4
#define pinBotao 15

unsigned long tempoAnterior = 0;
unsigned long tempoAtual;
BluetoothSerial BT; 

//F4:65:0B:54:70:12
uint8_t endslave[6] = {0x78, 0x42, 0x1C, 0x6B, 0xEF, 0x3E};

bool modoLed;
bool modosLeds;
Bounce btn = Bounce();

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(5000);

  pinMode(pinLed, OUTPUT); 
  btn.attach(pinBotao, INPUT_PULLUP); 
  btn.interval(25); 

  if (BT.begin("estado_aperto", true)) {

    Serial.println("Apertado");
    Serial.print("Endereco BLUT MASTER:  ");
    Serial.println(BT.getBtAddressString()); 

    if (BT.connect(endslave)) 
    {
      Serial.println("Conectado!");
    } else {
      Serial.println("Erro na conexão!");
    }

  } else {
    Serial.println("ERROR");
  }

}

void loop() {

  btn.update(); 
  tempoAtual = millis(); 

  
  if (btn.fell()) {
    modosLeds = !modosLeds;
    
    if (modosLeds == true) {
      Serial.println("Botão Liga!");
      BT.println("liga"); 
    }

    if (modosLeds == false) {
      Serial.println("Botão Desliga!");
      BT.println("desliga"); 
    }
  }

  
  if (btn.rose()) {
    if (btn.previousDuration() > 2000) 
    {
      Serial.println("Botão pisca!");
      BT.println("PISCAR"); 
    }
  }
  

  
  if (BT.available()) 
  {
    String mensagemRecebida = BT.readStringUntil('\n'); 
    mensagemRecebida.trim();
    Serial.println("Mensagem Recebida:" + mensagemRecebida); 

    if (mensagemRecebida.endsWith("liga"))
    {
      digitalWrite(pinLed, HIGH); 
      modoLed = false;
    }

    if (mensagemRecebida.endsWith("desliga"))
    {
      digitalWrite(pinLed, LOW); 
      modoLed = false;
    }

    if (mensagemRecebida.endsWith("PISCAR")) {
      modoLed = true; 
    }   
  }

 

  
  if (modoLed == true) {
    if (tempoAtual - tempoAnterior >= 500) {
      digitalWrite(pinLed, !digitalRead(pinLed));
      tempoAnterior = tempoAtual; 
    }
  }

  if (Serial.available())
  {
    String mensagemEnviar = Serial.readStringUntil('\n');
    mensagemEnviar.trim(); 
    BT.println(mensagemEnviar); 
    Serial.println("Mensagem Enviada:" + mensagemEnviar);
  }
}
