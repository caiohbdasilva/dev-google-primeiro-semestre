#include <Arduino.h>
#include <BluetoothSerial.h>
#define LED 32

String mensagemRecebida;

BluetoothSerial BT;

void setup()
{
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
    Serial.setTimeout(5000);

    if (BT.begin("EspCaioSlave")) // slave
    {
        Serial.println("Bluetooth iniciado com sucesso");
        Serial.print("Endereço bluetooth ESP: ");
        Serial.println(BT.getBtAddressString());
    }
    else
    {
        Serial.println("Erro ao iniciar o bluetooth");
    }
}
// F4:65:0B:48:5F:C2

void loop()
{
    digitalWrite(LED, LOW);
    if (BT.available())
    {
        mensagemRecebida = BT.readStringUntil('\n');
        mensagemRecebida.trim();
        Serial.printf("Mensagem recebida: %s", mensagemRecebida);
    }
    
    mensagemRecebida = "LED ON";
    mensagemRecebida.trim();
    if (mensagemRecebida.endsWith("ON"))
    {
        digitalWrite(LED, HIGH);
    }
}
