#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial BT;

uint8_t endSlave[] = {0x8C, 0xAA, 0xB5, 0x94, 0x26, 0x62}; // endereco do dispositivo a ser conectado

void setup()
{
  Serial.begin(9600); // inicia a Serial.
  pinMode(2, OUTPUT);

  //!                       ↓ indica que eh master
  if(BT.begin("ESPpietra", true)) // inicia a conecxao bluetooth
  {
    Serial.println("Bluetooth iniciado c sucesso!");
  }
  else
  {
    Serial.println("Erro na inicializacao do Bluetooth!");
  }

  if (BT.connect(endSlave)) // conecta ao dispositivo
  {
    Serial.println("conectado com sucesso!");
  }
  else
  {
    Serial.println("nao foi possivel conectar ao dispositivo");
  }

  //* Serial.println(BT.getBtAddressString());  //mostra o endereco fisico do bluetooth
}

void loop()
{
  static bool piscaLed = false;
  static bool estadoLed = false;
  static int Velocidade = 200;

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
      estadoLed = true;
      piscaLed = false;
    }
    else if (mensagemRecebida.equals("desliga"))
    {
      estadoLed = false;
      piscaLed = false;
    }
    else if (mensagemRecebida.equals("pisca"))
    {
      piscaLed = true;
    }
    else
    {
      Serial.println("Comando Nao Encontrado");
    }
  }

  if (piscaLed == true)
  {
    unsigned long tempoAtual = millis();
    static unsigned long tempoInicial = 0;

    if (tempoAtual - tempoInicial >= Velocidade)
    {
      estadoLed = !estadoLed;
      tempoInicial = tempoAtual;
    }
  }

  digitalWrite(2, estadoLed);
}
