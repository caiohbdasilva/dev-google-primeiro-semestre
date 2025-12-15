// /*
//------------------------ BLUETOOTH SLAVE ------------------------
#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5000);

if (BT.begin("EspCaioSlave"))//slave
{
    Serial.println("Bluetooth iniciado com sucesso");
    Serial.print("Endereço bluetooth ESP: ");
    Serial.println(BT.getBtAddressString());
} else {
    Serial.println("Erro ao iniciar o bluetooth");
}
}
// F4:65:0B:48:5F:C2


void loop() {
    if (BT.available())
    {
         String mensagemRecebida = BT.readStringUntil('\n');
         mensagemRecebida.trim();
         Serial.printf("Mensagem recebida: %s", mensagemRecebida);
     }

     if (Serial.available())
     {
         String mensagemEnviar = Serial.readStringUntil('\n');
         mensagemEnviar.trim();
         BT.println(mensagemEnviar);
     }
}
// */

/*
//------------------------ BLUETOOTH MASTER ------------------------
#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

//F4:65:0B:54:70:12
uint8_t endSlave[] = {0xF4, 0x65, 0x0B, 0x54, 0x70, 0x12};

void setup(){
    Serial.begin(9600);
    Serial.setTimeout(5000);

    if (BT.begin("EspMasterOdirlei", true))//master
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

void loop(){
    if (BT.available())
    {
        String mensagemRecebida = BT.readStringUntil('\n');
        mensagemRecebida.trim();
        Serial.printf("Mensagem recebida: %s", mensagemRecebida);
    }

    if (Serial.available())
    {
        String mensagemEnviar = Serial.readStringUntil('\n');
        mensagemEnviar.trim();
        BT.println(mensagemEnviar);
    }
}
// */