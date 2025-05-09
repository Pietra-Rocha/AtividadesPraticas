#include <Arduino.h>

//prototipo de funcoes 
void ligaLed(int pin);
void desligaLed(int pin);
void piscaLed(int pin, unsigned long tempo);
int somaDeDoisN(int numeroA, int numeroB);



void setup()
{
 pinMode(2, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
  piscaLed(2, 100);
  Serial.print(somaDeDoisN(4, 8));
}






void ligaLed(int pin)
{
digitalWrite(pin, HIGH);
}

void desligaLed(int pin)
{
digitalWrite(pin, LOW);
}

void piscaLed(int pin, unsigned long tempo)
{
ligaLed(pin);
  delay(tempo);
  desligaLed(pin);
  delay(tempo);
}

int somaDeDoisN(int numeroA, int numeroB)
{
int resultado = numeroA + numeroB;
return resultado;
}