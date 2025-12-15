#include <Arduino.h>
#include <BluetoothSerial.h>
 
 void setup() {

   Serial.begin(9600); 
   Serial.setTimeout(5000);  

 pinMode(4, OUTPUT); 
 pinMode (pinBotao, INPUT_PULLUP);

   if (BT.begin("dedinho_master", true)) {
    
     Serial.println("dedinho master incerido");
     Serial.print ("Endereco BLUT MASTER:  ");
     Serial.println(BT.getBtAddressString());  
     if (BT.connect(endslave))
       Serial.println("dedinho master conectado");
     } else {
       Serial.println("erro ao iniciar o dedinho");
     }
  
   } else {
     Serial.println("ERROR");
   } 

 }

 void loop() {

 tempoAtual = millis();

 if (BT.available())
 {
   String mensagemRecebida = BT.readStringUntil('\n'); 
   mensagemRecebida.trim();
   Serial.println("Mensagem Recebida:" + mensagemRecebida);
 
 if (mensagemRecebida.endsWith ("LIGAR"))
 {
   digitalWrite(4, HIGH);
 modoLed = false;

 } 

 if (mensagemRecebida.endsWith ("DESLIGAR"))
 {
   digitalWrite(4, LOW);
 modoLed = false;

 } 

 if (mensagemRecebida.endsWith("PISCAR")) {
             modoLed = true;  Ativa o modo piscar
          
         }
     }

     if (modoLed == true) { 
         if (tempoAtual - tempoAnterior >= 500) { 
             digitalWrite(pinLed, !digitalRead(pinLed)); 
             tempoAnterior = tempoAtual;  Atualiza o tempo anterior para o próximo piscar
         }

      
    
     }

 if(Serial.available())
 {
   String mensagemEnviar = Serial.readStringUntil('\n');
   mensagemEnviar.trim();  remove espaços em branco extras
   BT.println(mensagemEnviar);  envia a mensagem via Bluetooth
   Serial.println("Mensagem Enviada:" + mensagemEnviar);
 }



    

 }

