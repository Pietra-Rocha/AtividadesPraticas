#include <Arduino.h>
#include "Led.h"

Led ledESP(2);
Led ledAmarelo(18);
Led ledVermelho(19);
Led ledBranco(21);

void setup() 
{
 ledESP.ligar();
 ledAmarelo.piscar(5);
 ledVermelho.piscar(5);
 ledBranco.piscar(2);
}

void loop() 
{
 ledESP.update();
 ledAmarelo.update();
 ledVermelho.update();
 ledBranco.update();
}