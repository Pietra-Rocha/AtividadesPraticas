#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel ledRGB (1, 48, NEO_GRB);  //quantidade de led, pino e tipo

//variaveis globais
int r = 0;
int g = 0;
int b = 0;

void setup() 
{
  ledRGB.begin();
  ledRGB.setBrightness(32);  //intensidade do led varia de 0 - 255

  ledRGB.setPixelColor(0, ledRGB.Color(0, 0, 0));
  ledRGB.show();
}

void loop() 
{
  

  ledRGB.setPixelColor(0, ledRGB.Color(r, g, b));
  ledRGB.show();

  

}