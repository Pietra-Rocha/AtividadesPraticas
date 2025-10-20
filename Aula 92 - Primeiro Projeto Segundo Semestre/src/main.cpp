#include <Arduino.h>
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel ledRGB(1, 48, NEO_GRB); // quantidade de led, pino e tipo
Bounce botao = Bounce();

// variaveis globais
int r = 0;
int g = 0;
int b = 0;
int contador = 0; // Contador de toques

void setup()
{
  Serial.begin(9600);
  ledRGB.begin();
  ledRGB.setBrightness(32); // intensidade do led varia de 0 - 255

  ledRGB.setPixelColor(0, ledRGB.Color(0, 0, 0));
  ledRGB.show();

  pinMode(0, INPUT_PULLUP);
  botao.attach(0);
  botao.interval(25);
}

void loop()
{
  botao.update(); // Atualiza o estado do botão

  if (botao.fell())
  {             // Se o botão foi pressionado (borda de descida)
    contador++; // Incrementa o contador
    if (contador > 9)
    {
      contador = 0;
    }
    Serial.print("Botão pressionado: ");
    Serial.println(contador);

    // Altera a cor do LED em sequência (vermelho, verde, azul, apagado)
    if (r == 0 && g == 0 && b == 0)
    {
      r = 255;
      g = 0;
      b = 0;
    }
    else if (r == 255)
    {
      r = 0;
      g = 255;
      b = 0;
    }
    else if (g == 255)
    {
      r = 0;
      g = 0;
      b = 255;
    }
    else if (b == 255)
    {
      r = 216;
      g = 52;
      b = 235;
    }
    else if (r == 216 && g == 52 && b == 235)
    {
      r = 52;
      g = 235;
      b = 232;
    }
    else if (r == 52 && g == 235 && b == 232)
    {
      r = 235;
      g = 143;
      b = 52;
    }
    else if (r == 235 && g == 143 && b == 52)
    {
      r = 134;
      g = 52;
      b = 235;
    }
    else if (r == 134 && g == 52 && b == 235)
    {
      r = 217;
      g = 235;
      b = 52;
    }
    else if (r == 217 && g == 235 && b == 52)
    {
      r = 235;
      g = 52;
      b = 143;
    }
    else
    {
      r = 0;
      g = 0;
      b = 0; // Apaga o LED
    }
  }

  ledRGB.setPixelColor(0, ledRGB.Color(r, g, b));
  ledRGB.show();
}