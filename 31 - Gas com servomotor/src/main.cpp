#include <Arduino.h>
#include <ESP32Servo.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

#define led 25
Servo servoBase;
BluetoothSerial BT;

/*
//F4:65:0B:54:70:12
uint8_t endSlave[] = {0xF4, 0x65, 0x0B, 0x54, 0x70, 0x12};
*/



void setup() {
  //LED
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  //Servomotor

  servoBase.attach(5);//Pin a utilizar para servo
  servoBase.write(0);  //asigno 0 al servo motor


  //Bluetooth
  if (BT.begin("EspCaioSlave"))//slave
{
    Serial.println("Bluetooth iniciado com sucesso");
    Serial.print("Endereço bluetooth ESP: ");
    Serial.println(BT.getBtAddressString());
} else {
    Serial.println("Erro ao iniciar o bluetooth");
}


}

void loop() {

  //Bluetooth
 String mensagemRecebida = BT.readString();
    
    JsonDocument doc;

    DeserializationError erro = deserializeJson(doc, mensagemRecebida);

    Serial.print(mensagemRecebida);

  mensagemRecebida.trim();

  bool limite = doc["limite"];

  if (limite){
    servoBase.write(170);
    digitalWrite(led, HIGH);
  } else {
    servoBase.write(0);
    digitalWrite(led, LOW);
  }

}