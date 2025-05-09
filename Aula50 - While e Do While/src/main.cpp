#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define pinLed 2
#define botao 0

void setup()
{

  delay(5000);
  pinMode(2, OUTPUT);
  pinMode(0, INPUT_PULLUP);

  bool estadoBotao = digitalRead(0);

  while (estadoBotao == 0)
  { // enquanto
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
    estadoBotao = digitalRead(0);
  }
}

void loop()
{
}