#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop()
{
  static bool piscaLed = false;
  static bool estadoLed = false;
  static int  Velocidade = 200;

  if (Serial.available())
  {
    String texto = Serial.readStringUntil('\n'); // vou ler o serial ate ter um enter
    Serial.printf("Voce digitou: %s\n\r", texto);
    texto.trim();

    if (texto.equals("liga"))
    {
      estadoLed = true;
      piscaLed = false;
    }
    else if (texto.equals("desliga"))
    {
      estadoLed = false;
      piscaLed = false;
    }
    else if (texto.equals("pisca"))
    {
      piscaLed = true;
    }
    else if (texto.equals("mais rapido"))
    {
      piscaLed = true;
      Velocidade -= 100;
    }
    else if (texto.equals("mais devagar"))
    {
      piscaLed = true;
      Velocidade += 100;
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

//* LER APENAS UM CARACTER
// void loop()
// {
//   if (Serial.available())
//   {
//     char caracter = Serial.read();
//     Serial.printf("Recebido letra: %c\n\r", caracter);

//     if (caracter == 'a')
//     {
//       digitalWrite(2, HIGH);
//     }
//     else if (caracter == 'b')
//     {
//       digitalWrite(2, LOW);
//     }
//   }
// }