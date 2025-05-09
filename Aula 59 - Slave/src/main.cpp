#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

void setup()
{
  Serial.begin(9600); // inicia a Serial.

  if (BT.begin("ESPslavePietra"))
  {
    Serial.println("Bluettoth iniciado c sucesso!");
  }
  else
  {
    Serial.println("Erro na inicializacao");
  }
}

void loop()
{
  if (Serial.available())
  {
    String mensagemEnviar = Serial.readStringUntil('\n'); // ler a Serial ate encontrar um enter
    mensagemEnviar.trim();                                // tratamento da msm
    BT.println(mensagemEnviar);                           // enviar a msm ao bluetooth
  }

  if (BT.available())
  {
    String mensagemRecebida = BT.readStringUntil('\n');      // ler os caracteres recebidos no bluettoth ate encontrar um enter
    mensagemRecebida.trim();                                 // tratar a mensagem
    Serial.println("Mensagem recebida " + mensagemRecebida); // enviar ao serial que recebeu a msm

    if (mensagemRecebida.equals("liga"))
    {
    }
    else if (mensagemRecebida.equals("desliga"))
    {
    }
  }
}
