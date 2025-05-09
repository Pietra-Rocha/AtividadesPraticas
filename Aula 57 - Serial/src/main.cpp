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
  static int Velocidade = 200;

  if (Serial.available())
  {
    String texto = Serial.readStringUntil('\n'); // vou ler o serial ate ter um enter
    Serial.printf("Voce digitou: %s\n\r", texto);
    texto.trim();
    texto.toLowerCase();

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
      if (Velocidade < 50)
      {
        Velocidade = 50;
        Serial.println("velocidade maxima atingida");
      }
    }
    else if (texto.equals("mais devagar"))
    {
      piscaLed = true;
      if (Velocidade > 1000)
      {
        Velocidade = 1000;
        Serial.println("velocidade minima atingida");
      }
    }
    else if (texto.startsWith("velocidade = "))
    {
      int tamanho = texto.length();
      String extracao = texto.substring(13, tamanho);
      int vel = extracao.toInt();

      if (vel >= 0 && vel <= 100)
      {
        Velocidade = map(vel, 0, 100, 1000, 50);
      }
      else
      {
        Serial.println("valores aceitos somente entre 0 e 100");
      }
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